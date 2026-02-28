#include "MPU6050Wrapper.h"
#include <Arduino.h>

// ── Static member ─────────────────────────────────────────────────────────────
volatile bool MPU6050Wrapper::_mpuInterrupt = false;

void MPU6050Wrapper::dmpDataReady()
{ // static ISR
    _mpuInterrupt = true;
}

// ── Constructor ───────────────────────────────────────────────────────────────
MPU6050Wrapper::MPU6050Wrapper()
    : _dmpReady(false), _newDMPData(false), _devStatus(0), _packetSize(0)
{
    memset(&_orient, 0, sizeof(_orient));
    memset(&_raw, 0, sizeof(_raw));

    // Teapot packet template
    uint8_t tpl[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n'};
    memcpy(_teapotPacket, tpl, 14);
}

// ── begin() ───────────────────────────────────────────────────────────────────
bool MPU6050Wrapper::begin(bool calibrate)
{

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000);
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    Serial.println(F("Initializing I2C devices..."));
    _mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.println(F("Testing device connections..."));
    Serial.println(_mpu.testConnection()
                       ? F("MPU6050 connection successful")
                       : F("MPU6050 connection failed"));

    // ── DMP init ──────────────────────────────────────────────────────────────
    Serial.println(F("Initializing DMP..."));
    _devStatus = _mpu.dmpInitialize();

    // Supply your own gyro offsets here (from IMU_Zero / calibration sketch).
    // These are reasonable starting defaults; run IMU_Zero.ino to find yours.
    _mpu.setXGyroOffset(220);
    _mpu.setYGyroOffset(76);
    _mpu.setZGyroOffset(-85);
    _mpu.setZAccelOffset(1788);

    if (_devStatus != 0)
    {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(_devStatus);
        Serial.println(F(")"));
        return false;
    }

    // ── Optional PID calibration ──────────────────────────────────────────────
    if (calibrate)
    {
        Serial.println(F("Calibrating... (each dot = 100 readings)"));
        _mpu.CalibrateAccel(6);
        _mpu.CalibrateGyro(6);
        printOffsets();
    }

    // ── Enable DMP & interrupt ────────────────────────────────────────────────
    Serial.println(F("Enabling DMP..."));
    _mpu.setDMPEnabled(true);

    Serial.print(F("Enabling interrupt on pin "));
    Serial.println(digitalPinToInterrupt(INTERRUPT_PIN));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

    _packetSize = _mpu.dmpGetFIFOPacketSize();
    _dmpReady = true;

    Serial.println(F("DMP ready!"));
    return true;
}

// ── update() ──────────────────────────────────────────────────────────────────
void MPU6050Wrapper::update()
{
    // Always grab raw register data
    _updateRaw();

    // Only attempt DMP read when the interrupt has fired
    if (!_dmpReady || !_mpuInterrupt)
        return;
    _mpuInterrupt = false;

    _updateDMP();
}

// ── _updateRaw() ──────────────────────────────────────────────────────────────
void MPU6050Wrapper::_updateRaw()
{
    _mpu.getMotion6(
        &_raw.ax, &_raw.ay, &_raw.az,
        &_raw.gx, &_raw.gy, &_raw.gz);
}

// ── _updateDMP() ─────────────────────────────────────────────────────────────
void MPU6050Wrapper::_updateDMP()
{
    if (!_mpu.dmpGetCurrentFIFOPacket(_fifoBuffer))
        return;

    _newDMPData = true;

    // Quaternion is needed by almost every output mode
    _mpu.dmpGetQuaternion(&_q, _fifoBuffer);
    _orient.q = _q;

#ifdef OUTPUT_READABLE_EULER
    float euler[3];
    _mpu.dmpGetEuler(euler, &_q);
    _orient.euler[0] = euler[0] * 180.0f / M_PI;
    _orient.euler[1] = euler[1] * 180.0f / M_PI;
    _orient.euler[2] = euler[2] * 180.0f / M_PI;
#endif

#if defined(OUTPUT_READABLE_YAWPITCHROLL) || \
    defined(OUTPUT_READABLE_REALACCEL) ||    \
    defined(OUTPUT_READABLE_WORLDACCEL)
    _mpu.dmpGetGravity(&_gravity, &_q);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
    float ypr[3];
    _mpu.dmpGetYawPitchRoll(ypr, &_q, &_gravity);
    _orient.yaw = ypr[0] * 180.0f / M_PI;
    _orient.pitch = ypr[1] * 180.0f / M_PI;
    _orient.roll = ypr[2] * 180.0f / M_PI;
#endif

#if defined(OUTPUT_READABLE_REALACCEL) || defined(OUTPUT_READABLE_WORLDACCEL)
    _mpu.dmpGetAccel(&_aa, _fifoBuffer);
    _mpu.dmpGetLinearAccel(&_orient.aaReal, &_aa, &_gravity);
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
    _mpu.dmpGetLinearAccelInWorld(&_orient.aaWorld, &_orient.aaReal, &_q);
#endif

#ifdef OUTPUT_TEAPOT
    _teapotPacket[2] = _fifoBuffer[0];
    _teapotPacket[3] = _fifoBuffer[1];
    _teapotPacket[4] = _fifoBuffer[4];
    _teapotPacket[5] = _fifoBuffer[5];
    _teapotPacket[6] = _fifoBuffer[8];
    _teapotPacket[7] = _fifoBuffer[9];
    _teapotPacket[8] = _fifoBuffer[12];
    _teapotPacket[9] = _fifoBuffer[13];
    _teapotPacket[11]++;
#endif
}

// ── printOffsets() ────────────────────────────────────────────────────────────
void MPU6050Wrapper::printOffsets()
{
    Serial.println(F("Active offsets:"));
    _mpu.PrintActiveOffsets();
    Serial.println();
}

// ── printData() ───────────────────────────────────────────────────────────────
void MPU6050Wrapper::printData()
{
    _printDMPData();
    _printRawData();

    // Blink LED to signal activity
    static bool ledState = false;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
}

void MPU6050Wrapper::_printDMPData()
{
    if (!_newDMPData)
        return;
    _newDMPData = false;

#ifdef OUTPUT_READABLE_QUATERNION
    Serial.print(F("quat\t"));
    Serial.print(_orient.q.w);
    Serial.print(F("\t"));
    Serial.print(_orient.q.x);
    Serial.print(F("\t"));
    Serial.print(_orient.q.y);
    Serial.print(F("\t"));
    Serial.println(_orient.q.z);
#endif

#ifdef OUTPUT_READABLE_EULER
    Serial.print(F("euler\t"));
    Serial.print(_orient.euler[0]);
    Serial.print(F("\t"));
    Serial.print(_orient.euler[1]);
    Serial.print(F("\t"));
    Serial.println(_orient.euler[2]);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
    Serial.print(F("ypr\t"));
    Serial.print(_orient.yaw);
    Serial.print(F("\t"));
    Serial.print(_orient.pitch);
    Serial.print(F("\t"));
    Serial.println(_orient.roll);
#endif

#ifdef OUTPUT_READABLE_REALACCEL
    Serial.print(F("areal\t"));
    Serial.print(_orient.aaReal.x);
    Serial.print(F("\t"));
    Serial.print(_orient.aaReal.y);
    Serial.print(F("\t"));
    Serial.println(_orient.aaReal.z);
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
    Serial.print(F("aworld\t"));
    Serial.print(_orient.aaWorld.x);
    Serial.print(F("\t"));
    Serial.print(_orient.aaWorld.y);
    Serial.print(F("\t"));
    Serial.println(_orient.aaWorld.z);
#endif

#ifdef OUTPUT_TEAPOT
    Serial.write(_teapotPacket, 14);
#endif
}

void MPU6050Wrapper::_printRawData()
{
#ifdef OUTPUT_READABLE_ACCELGYRO
    Serial.print(F("a/g:\t"));
    Serial.print(_raw.ax);
    Serial.print(F("\t"));
    Serial.print(_raw.ay);
    Serial.print(F("\t"));
    Serial.print(_raw.az);
    Serial.print(F("\t"));
    Serial.print(_raw.gx);
    Serial.print(F("\t"));
    Serial.print(_raw.gy);
    Serial.print(F("\t"));
    Serial.println(_raw.gz);
#endif

#ifdef OUTPUT_BINARY_ACCELGYRO
    Serial.write((uint8_t)(_raw.ax >> 8));
    Serial.write((uint8_t)(_raw.ax & 0xFF));
    Serial.write((uint8_t)(_raw.ay >> 8));
    Serial.write((uint8_t)(_raw.ay & 0xFF));
    Serial.write((uint8_t)(_raw.az >> 8));
    Serial.write((uint8_t)(_raw.az & 0xFF));
    Serial.write((uint8_t)(_raw.gx >> 8));
    Serial.write((uint8_t)(_raw.gx & 0xFF));
    Serial.write((uint8_t)(_raw.gy >> 8));
    Serial.write((uint8_t)(_raw.gy & 0xFF));
    Serial.write((uint8_t)(_raw.gz >> 8));
    Serial.write((uint8_t)(_raw.gz & 0xFF));
#endif
}