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
        splitter.extend();
        chassis.pid_odom_set({{-9_in, 26_in, -20_deg}, forward, 80});
        chassis.pid_wait_until(18);
        scraper.extend();
        chassis.pid_wait();
        
        chassis.pid_turn_set(-125_deg, 127);
        chassis.pid_wait();

        chassis.pid_odom_set({
            {{-20_in, 10_in}, forward, 127},
            {{-32_in, -5_in, -180_deg}, forward, 127}
        }, true);
        chassis.pid_wait_quick();
        chassis.pid_drive_set(30_in, 127);
        
        pros::delay(250);
        chassis.pid_drive_set(-1_in, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(1_in, 127);
        chassis.pid_wait();
        pros::delay(1500);

        chassis.pid_drive_set(-28_in, 127);
        // chassis.pid_odom_set({{-32_in, 20_in, -180_deg}, reverse, 127});
        chassis.pid_wait_until(3_in);
        scraper.retract();
        intake::score_low();
        pros::delay(250);
        intake::load_bot();
        chassis.pid_wait_quick();

        intake::toggle_invert_mode(true);
        intake::set_load_speed(127);
        intake::load_bot();
        pros::delay(1000);
        intake::set_load_speed(100);
    }
    void right() {
        reset_pos();
        splitter.extend();
        intake::load_bot();
        chassis.pid_odom_set({{0_in, 18_in}, fwd, 70});
        chassis.pid_turn_set(33_deg, 127);
        chassis.pid_wait();
        chassis.pid_odom_set({{7_in, 28_in}, fwd, 50});
        pros::delay(2500);


        chassis.pid_turn_set(-225_deg, 127);
        scraper.extend();
        chassis.pid_odom_set({{28_in, -13.5_in, 175_deg}, forward, 127});
        chassis.pid_wait();
        chassis.pid_drive_set(2_in, 127);
        chassis.pid_wait();
        
    }
}