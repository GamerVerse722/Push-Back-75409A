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
    void unified() {
        reset_pos();
        chassis.odom_theta_set(-90_deg);
        // Sets the robot's position and activates pneumatics
        splitter.extend();
        descore.extend();
        scraper.extend();

        // Lines up the robot with the match loader and the high goal
        chassis.pid_odom_set({{-30_in, 0_in, -90_deg}, fwd, 127});
        chassis.pid_wait();
        chassis.pid_turn_set(-180_deg, 127);
        chassis.pid_wait();

        // Has the robot drive up to the match loader and pick up three of the balls
        intake::load_bot();
        chassis.pid_drive_set(11.75_in, 80);
        chassis.pid_wait();
        pros::delay(250);

        // Has the robot drive up to and score 4 balls on the high goal
        chassis.pid_drive_set(-29_in, 127);
        chassis.pid_wait();
        intake::score_high();
        pros::delay(1500);
    }

    void left() {
        chassis.odom_theta_set(-90_deg);
        unified();
        // Has the robot drive to the side of the goal and push the balls toward the center with the descore mech
        Eliminations::unified_descore();
        // chassis.pid_wait();
    }

    void right() {
        chassis.odom_x_flip(true);
        chassis.odom_theta_flip(true);
        unified();
        // chassis.odom_theta_flip(false);
        // Eliminations::unified_descore();
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
        chassis.pid_drive_set(-30_in, 127);
        chassis.pid_wait_until(-3_in);
        scraper.retract();
        intake::score_low();
        pros::delay(200);
        intake::load_bot();
        pros::delay(1500);
        chassis.pid_drive_set(1_in, 127);
        chassis.pid_wait_quick();

        // Score High
        intake::score_high();
        pros::delay(2250);
    }

    void unified_descore() {
        // Push with doinker
        intake::stop();
        descore.retract();
        chassis.pid_drive_set(12_in, 127);
        chassis.pid_wait();
        chassis.pid_turn_set(-225_deg, 127);
        chassis.pid_wait();
        chassis.pid_drive_set(-16_in, 127);
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
    }

    void right() {
        reset_pos();
        chassis.odom_theta_set(90_deg);
        intake::set_low_score_speed(-100);
        
        splitter.extend();
        // descore.extend();

        // Go to Red Right Goal
        intake::load_bot();
        scraper.extend();
        chassis.pid_odom_set({{30_in, 0_in, 90_deg}, fwd, 127});
        chassis.pid_wait();
        chassis.pid_turn_set(180_deg, 127);
        chassis.pid_wait();

        // nav.record();
        chassis.pid_drive_set(20_in, 80);
        pros::delay(1200);
        // nav.reset_y();

        // Go to Red Right Long Goal
        chassis.pid_drive_set(-30_in, 127);
        chassis.pid_wait_until(-4_in);
        scraper.retract();
        chassis.pid_wait();
        intake::score_high();
        pros::delay(1400);
        intake::load_bot();
        
        // Leave Long Goal
        chassis.pid_swing_set(ez::LEFT_SWING, -35_deg, 127, -10);
        chassis.pid_wait();

        // Go Grab 6 Balls first left then right
        chassis.pid_odom_set({{-3_in, 34_in, -45_deg}, fwd, 127});
        chassis.pid_wait();
        intake::score_low();
        pros::delay(1500);
        intake::stop();

        nav.drive_to_x(24.5_in, 127, false);
        chassis.pid_turn_set(180_deg, 127);
        chassis.pid_wait();
        chassis.pid_odom_set({{23.5_in, 35_in, 180_deg}, rev, 127});
        chassis.pid_wait();
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