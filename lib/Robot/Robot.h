#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "motor_control.h"
#include "IR_control.h"

/**
 * @class Robot
 * @brief Simplified robot control interface for the ELEGOO OwlBot car
 *
 * Provides basic movement and IR tracking sensor access
 */
class Robot
{
public:
    /**
     * @enum Direction
     * @brief Robot movement directions
     */
    enum Direction
    {
        FORWARD,        // Move forward
        BACKWARD,       // Move backward
        LEFT,           // Turn left
        RIGHT,          // Turn right
        LEFT_FORWARD,   // Diagonal forward-left
        LEFT_BACKWARD,  // Diagonal backward-left
        RIGHT_FORWARD,  // Diagonal forward-right
        RIGHT_BACKWARD, // Diagonal backward-right
        STOP            // Stop all motors
    };

    /**
     * @brief Initialize the robot (motors and IR sensors)
     */
    void init();

    /**
     * @brief Move robot in specified direction at given speed
     * @param direction Direction to move (from Direction enum)
     * @param speed Motor speed 0-255
     */
    void move(Direction direction, uint8_t speed = 200);

    /**
     * @brief Stop the robot
     */
    void stop();

    /**
     * @brief Get raw IR sensor readings
     * @param left_out Output for left sensor value (0-1024)
     * @param middle_out Output for middle sensor value (0-1024)
     * @param right_out Output for right sensor value (0-1024)
     */
    void getIRSensorValues(uint16_t &left_out, uint16_t &middle_out, uint16_t &right_out);

    /**
     * @brief Check if robot is on the line (between thresholds)
     * @param value Sensor value to check
     * @return true if value is within line detection thresholds
     */
    boolean isOnLine(uint16_t value);

    /**
     * @brief Set IR sensor thresholds for line detection
     * @param start Lower threshold
     * @param end Upper threshold
     */
    void setLineThresholds(uint16_t start, uint16_t end);

    /**
     * @brief Get current IR sensor threshold start value
     * @return Lower threshold
     */
    uint16_t getThresholdStart() const { return _threshold_start; }

    /**
     * @brief Get current IR sensor threshold end value
     * @return Upper threshold
     */
    uint16_t getThresholdEnd() const { return _threshold_end; }

private:
    // Motor driver instance
    DeviceDriverSet_Motor _motor;

    // IR sensor driver instance
    DeviceDriverSet_STM8S003F3_IR _ir_sensor;

    // IR sensor tracking thresholds for line detection
    uint16_t _threshold_start = 500;
    uint16_t _threshold_end = 850;
};

#endif // ROBOT_H
