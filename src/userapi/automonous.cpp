#include "userapi/automonous.hpp"

#include "EZ-Template/util.hpp"
#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;
using namespace keybindActions;
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
        // chassis.pid_odom_set({{0_in, 24_in, 0_deg}, forward, 127});
        // chassis.pid_wait();

        // chassis.pid_odom_set({{16_in, 0_in, 0_deg}, reverse, 127});
        // chassis.pid_wait();

        // chassis.pid_odom_set({{16_in, 8_in}, forward, 127});
        // chassis.pid_wait_quick();
        // chassis.pid_turn_set(-90_deg, 127);
        // chassis.pid_wait_quick();

        // chassis.pid_odom_set({{0_in, 8_in}, forward, 127});
        // chassis.pid_wait_quick();
        // chassis.pid_turn_set(-180, 127);
        // chassis.pid_wait_quick();
        // chassis.pid_odom_set({{0_in, 0_in}, forward, 127});
        // chassis.pid_wait_quick();
    }
    void right() {
    }
}

namespace autom::Eliminations {
    void left() {
        reset_pos();
        intake::load_bot();
        chassis.pid_odom_set({{0_in, 24_in, -45_deg}, forward, 127});
        chassis.pid_wait();

        chassis.pid_turn_set(-125_deg, 127);
        chassis.pid_wait();

        chassis.pid_odom_set({
            {{-20_in, 10_in}, forward, 127},
            {{-24_in, -10_in, -180_deg}, forward, 127}
        }, true);
        chassis.pid_wait_until_index(1);
        scraper.extend();
        chassis.pid_wait();
        pros::delay(1000);

        chassis.pid_odom_set({{-24_in, 24_in, -180_deg}, reverse, 127});
        chassis.pid_wait();

        intake::score_high();
    }
    void right() {
        reset_pos();
        splitter.extend();
        intake::load_bot();
        chassis.pid_odom_set({{0_in, 20_in}, fwd, 70});
        chassis.pid_turn_set(25_deg, 127);
        chassis.pid_wait();
        chassis.pid_odom_set({{6_in, 28_in}, fwd, 70});

        chassis.pid_wait();

        //chassis.pid_turn_set(-125_deg, 127);
        //chassis.pid_wait();
    }
}