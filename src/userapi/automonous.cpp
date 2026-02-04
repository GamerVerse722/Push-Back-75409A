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
        chassis.pid_drive_set(2_in, 50);
        chassis.pid_wait();
    }

    void right() {
        reset_pos();
        chassis.pid_drive_set(2_in, 50);
        chassis.pid_wait();
    }
}

namespace autom::Eliminations {
    // Designed using left side cords
    void unified() {
        reset_pos();
        // Grab 3 balls
        intake::load_bot();
        splitter.extend();
        chassis.pid_odom_set({{-9_in, 26_in, -20_deg}, forward, 80});
        chassis.pid_wait_until(18);
        // scraper.extend();
        chassis.pid_wait();
        
        // Grab balls from loaders
        nav.drive_to_x(-35_in, -135_deg, 110);
        // nav.drive_to_x(-10_in, -135_deg, 127);
        // return;
        chassis.pid_turn_set(-180_deg, 127);
        scraper.extend();
        chassis.pid_wait();

        chassis.pid_drive_set(20_in, 100);
        pros::delay(2000);
        // chassis.pid_turn_set(-125_deg, 127);
        // chassis.pid_wait();
        // scraper.extend();

        // chassis.pid_odom_set({
        //     {{-20_in, 10_in}, forward, 127},
        //     {{-34_in, -5_in, -180_deg}, forward, 127}
        // }, true);
        // chassis.pid_wait_quick();
        // chassis.pid_drive_set(30_in, 127);
        // pros::delay(2000);

        // return;

        // Move to score high goals
        chassis.pid_drive_set(-28_in, 127);
        chassis.pid_wait_until(-3_in);
        scraper.retract();
        intake::score_low();
        pros::delay(200);
        intake::load_bot();
        chassis.pid_wait_quick();

        // Score High
        intake::score_high();
        pros::delay(2250);
    }

    void unified_descore() {
        // Push with doinker
        intake::stop();
        chassis.pid_drive_set(12_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-225_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-15_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-180_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-20_in, 127);
        chassis.pid_wait();
    }

    void left() {
        unified();
        // unified_descore();
    }
    void right() {
        chassis.odom_x_flip(true);
        chassis.odom_theta_flip(true);
        unified();
        chassis.odom_theta_flip(false);
        // unified_descore();
    }
}

namespace autom::Skills {
    void skills() {
        reset_pos();

        // Grab Balls
        splitter.extend();
        chassis.pid_drive_set(28_in, 100);
        chassis.pid_wait();
        chassis.pid_turn_set(-90_deg, 127);
        scraper.extend();
        intake::load_bot();
        chassis.pid_wait();
        chassis.pid_drive_set(48_in, 100);
        pros::delay(2000);

        // Score High
        chassis.pid_turn_set(-90_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-48_in, 127);
        chassis.pid_wait_until(-3_in);
        scraper.retract();
        descore.extend();
        chassis.pid_wait();
        intake::score_high();
        pros::delay(2000);
        intake::load_bot();

        // return;

        // Go to opposite side
        chassis.pid_drive_set(24_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-135_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-17_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-90_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-68_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-45_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-16_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(90_deg, 127);

        // return;
        
        // Grab Balls and score
        scraper.extend();
        chassis.pid_wait();
        chassis.pid_drive_set(48_in, 120);
        pros::delay(2000);
        chassis.pid_drive_set(-48_in, 127);
        chassis.pid_wait_until(-3_in);
        scraper.retract();
        chassis.pid_wait();
        intake::score_high();
        pros::delay(2000);

        return;

        // Parking 
        chassis.pid_drive_set(-12_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-135_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(15_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-90_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(60_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-180_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(48_in, 127);
        chassis.pid_wait();
    }
}

namespace autom::None {
    void none() {}
}