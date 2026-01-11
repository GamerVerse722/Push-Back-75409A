#include "userapi/automonous.hpp"

#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;

const int DRIVE_SPEED = 110;
const int TURN_SPEED = 127;
const int SWING_SPEED = 110;

void reset_pos() {
    chassis.pid_targets_reset();                // Resets PID targets to 0
	chassis.drive_imu_reset();                  // Reset gyro position to 0
	chassis.drive_sensor_reset();               // Reset drive sensors to 0
	chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
}

namespace autom::Qualifications {
    void left() {
        reset_pos();
        // chassis.pid_drive_set(12_in, 127);
        chassis.pid_turn_set(90_deg, TURN_SPEED);
        chassis.pid_wait();
        chassis.pid_turn_set(0_deg, TURN_SPEED);
        chassis.pid_wait();
    }
    void right() {
    }
}

namespace autom::Eliminations {
    void left() {
    }
    void right() {
    }
}