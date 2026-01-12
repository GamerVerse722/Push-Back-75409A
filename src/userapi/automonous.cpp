#include "userapi/automonous.hpp"

#include "EZ-Template/util.hpp"
#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;
using namespace ez;

const int DRIVE_SPEED = 127;
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
        chassis.pid_odom_set({{0_in, 24_in, 0_deg}, forward, 127});
        chassis.pid_wait();

        chassis.pid_odom_set({{16_in, 0_in, 0_deg}, reverse, 127});
        chassis.pid_wait();

        chassis.pid_odom_set({{16_in, 8_in}, forward, 127});
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 127);
        chassis.pid_wait_quick();

        chassis.pid_odom_set({{0_in, 8_in}, forward, 127});
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-180, 127);
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{0_in, 0_in}, forward, 127});
        chassis.pid_wait_quick();
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