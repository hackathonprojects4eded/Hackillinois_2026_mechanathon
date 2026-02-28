#pragma once

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "SimpleKalmanFilter.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// ── Output mode flags ────────────────────────────────────────────────────────
// Define ONE or MORE of these before including this header (or here directly).
// If none are defined, OUTPUT_READABLE_YAWPITCHROLL and OUTPUT_READABLE_ACCELGYRO
// are enabled by default.

// #define OUTPUT_READABLE_QUATERNION
// #define OUTPUT_READABLE_EULER
#define OUTPUT_READABLE_YAWPITCHROLL
// #define OUTPUT_READABLE_REALACCEL
// #define OUTPUT_READABLE_WORLDACCEL
// #define OUTPUT_TEAPOT
// #define OUTPUT_READABLE_ACCELGYRO // raw accel/gyro (non-DMP)
// #define OUTPUT_BINARY_ACCELGYRO

// ── Pin config ────────────────────────────────────────────────────────────────
#define INTERRUPT_PIN 2
#define LED_PIN 13

// ── Public data structures ────────────────────────────────────────────────────

struct RawData
{
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
};

struct OrientationData
{
    float yaw, pitch, roll; // degrees  (YPR mode)
    float euler[3];         // degrees  (Euler mode)
    Quaternion q;           // (Quaternion mode)
    VectorInt16 aaReal;     // gravity-free accel
    VectorInt16 aaWorld;    // world-frame accel
};

// ── MPU6050Wrapper class ──────────────────────────────────────────────────────

class MPU6050Wrapper
{
public:
    MPU6050Wrapper();

    // Call once in Arduino setup().
    // Returns true if DMP initialised successfully.
    bool begin(bool calibrate = true);

    // Call every Arduino loop().
    // Reads DMP FIFO (if ready) AND raw registers, updates internal state.
    void update();

    // ── Accessors ────────────────────────────────────────────────────────────

    // DMP-derived orientation (valid after update() when dmpReady())
    const OrientationData &orientation() const { return _orient; }

    // Raw register values (valid after every update())
    const RawData &raw() const { return _raw; }

    bool dmpReady() const { return _dmpReady; }
    bool newDMPData() const { return _newDMPData; } // cleared after read

    // Print the currently active offsets to Serial
    void printOffsets();

    // Print latest data to Serial according to enabled OUTPUT_* defines
    void printData();

    // Kalman-filtered orientation (updated after DMP update)
    float getFilteredYaw() const { return _filteredYaw; }
    float getFilteredPitch() const { return _filteredPitch; }
    float getFilteredRoll() const { return _filteredRoll; }

    // Direct access to the underlying driver
    MPU6050 &device() { return _mpu; }

    // ISR – attach to INTERRUPT_PIN externally, or call begin() which does it
    static void dmpDataReady();

private:
    MPU6050 _mpu;
    bool _dmpReady;
    bool _newDMPData;
    uint8_t _devStatus;
    uint16_t _packetSize;
    uint8_t _fifoBuffer[64];

    // DMP working vars
    Quaternion _q;
    VectorInt16 _aa;
    VectorFloat _gravity;

    OrientationData _orient;
    RawData _raw;

    // Kalman filters for pitch and roll
    SimpleKalmanFilter _kalmanPitch{1.0f, 1.0f, 0.01f};
    SimpleKalmanFilter _kalmanRoll{1.0f, 1.0f, 0.01f};
    float _filteredYaw, _filteredPitch, _filteredRoll;
    uint32_t _lastUpdateTime;

    // Teapot packet
    uint8_t _teapotPacket[14];

    void _updateDMP();
    void _updateRaw();
    void _printDMPData();
    void _printRawData();

    static volatile bool _mpuInterrupt;
};