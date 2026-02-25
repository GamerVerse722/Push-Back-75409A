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

void print_debug() {
    
}

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
    }

    void right() {
    }
}

namespace autom::Eliminations {
    // Designed using left side cords
    void unified() {
        reset_pos();
        // Grab 3 balls
        intake::load_bot();
        splitter.extend();
        chassis.pid_odom_set({{-9_in, 26_in, -20_deg}, forward, 50});
        chassis.pid_wait_until(18);
        // scraper.extend();
        chassis.pid_wait();
        pros::delay(250);
        
        // Grab balls from loaders
        nav.drive_to_x(-35_in, -135_deg, 100);
        // nav.drive_to_x(-10_in, -135_deg, 127);
        // return;
        chassis.pid_turn_set(-180_deg, 127);
        scraper.extend();
        chassis.pid_wait();

        chassis.pid_drive_set(20_in, 80);
        pros::delay(1800);
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
        chassis.pid_drive_set(-20_in, 100);
        chassis.pid_wait();
    }

    void left() {
        unified();
        unified_descore();
    }
    void right() {
        chassis.odom_x_flip(true);
        chassis.odom_theta_flip(true);
        unified();
        chassis.odom_theta_flip(false);
        unified_descore();
    }
}

namespace autom::AWP {
    void left() {
        reset_pos();
        chassis.odom_theta_set(-90_deg);
        splitter.extend();
        descore.extend();

        chassis.pid_odom_set({{-30_in, 0_in, -90_deg}, fwd, 127});
        chassis.pid_wait();
        chassis.pid_turn_set(-90_deg, 127);
        chassis.pid_wait();
    }
    void right() {
        reset_pos();
        chassis.odom_theta_set(90_deg);
        splitter.extend();
        descore.extend();

        // Go to Red Right Goal
        scraper.extend();
        chassis.pid_odom_set({{30_in, 0_in, 90_deg}, fwd, 127});
        chassis.pid_wait();
        chassis.pid_turn_set(180_deg, 127);
        chassis.pid_wait();

        nav.record();
        chassis.pid_drive_set(20_in, 80);
        pros::delay(1800);
        chassis.pid_drive_set(-28_in, 127);
    }
}

namespace autom::Skills {
    PROSLogger::Logger log{"Autonomous"};

    void print_debug() {
        log.debug(std::format("X, {:.2f}, Y, {:.2f}, Theta, {:.2f}, Distance: {:.2f}", chassis.odom_x_get(), chassis.odom_y_get(), chassis.odom_theta_get(), distance.get() / 25.4));
    }

    void skills() {
        reset_pos();
        chassis.odom_theta_set(-90_deg);
        splitter.extend();
        descore.extend();
        intake::load_bot();

        // Go to RED LEFT Goal
        chassis.pid_odom_set({{-29_in, 0_in, -90_deg}, fwd, 127});
        chassis.pid_wait();
        chassis.pid_turn_set(-180_deg, 127);
        scraper.extend();
        pros::delay(250);

        nav.record();
        chassis.pid_drive_set(13_in, 110);
        // chassis.pid_odom_set({{-30_in, -10_in, -180_deg}, fwd, 127});
        chassis.pid_wait();
        pros::delay(1200);
        nav.reset_y();

        // Go to other side of field
        chassis.pid_odom_set({
            {{-40_in, 40_in, -180_deg}, rev, 127},
            {{-40_in, 100_in, -180_deg}, rev, 127},
            // {{-26_in, 125_in, -180_deg}, rev, 100}
        }, true);

        chassis.pid_wait_until(-5_in);
        scraper.retract();

        chassis.pid_wait();

        nav.drive_to_x(-26_in, -136_deg, 127);
        nav.drive_to_y(100_in, 0_deg, 127);
        intake::score_high();
        pros::delay(2500);
        scraper.extend();
        intake::load_bot();
        chassis.pid_drive_set(36_in, 127);
        chassis.pid_wait();
        pros::delay(2500);
        nav.drive_to_y(100_in, 0_deg, 127);
        intake::score_high();
        pros::delay(2500);
    }

    void skills2() {
        reset_pos();
        intake::score_high();
        scraper.extend();
        chassis.pid_drive_set(-5_in, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(29_in, 127);
        chassis.pid_wait();
        scraper.retract();
    }
}

namespace autom::None {
    void none() {
        reset_pos();
        chassis.pid_drive_set(2_in, 50);
        chassis.pid_wait();
    }
}