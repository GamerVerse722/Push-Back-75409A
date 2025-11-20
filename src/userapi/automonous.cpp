#include "userapi/automonous.hpp"

// #include "lemlib/pose.hpp"
#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;

namespace autom {
    void skills() {}

    void redLeft() {
        //score middle
        chassis.setPose(0, 0, 90);
        keybindActions::intake::load_bot();
        chassis.moveToPose(28, 12, 50, 2000, {.minSpeed=70, .earlyExitRange=4});
        chassis.swingToHeading(-45, lemlib::DriveSide::LEFT, 1000, {.earlyExitRange=20});
        chassis.moveToPose(32, -2, -45, 1500, {.forwards=false});
        pros::delay(1000);
        keybindActions::intake::score_high();
        pros::delay(1800);
        keybindActions::intake::load_bot();

        // Grab Score
        // chassis.moveToPose(0, 23, -60, 2000, {.minSpeed=70, .earlyExitRange=4});
        devices::scraper.extend();
        chassis.moveToPose(-22, 30, -90, 2500, {.minSpeed=75});
        chassis.moveToPose(-35, 30, -90, 1300, {.minSpeed=110});
        devices::splitter.extend();
        chassis.moveToPose(10, 31, -90, 2000, {.forwards=false});
        keybindActions::intake::toggle_invert_mode(true);
        keybindActions::intake::set_load_speed(127);
        keybindActions::intake::load_bot();
    }
    
    void redRight() {
        chassis.setPose(0, 0, 90);
        keybindActions::intake::load_bot();
        chassis.moveToPose(28, -12, 130, 2000, {.minSpeed=70, .earlyExitRange=4});
        chassis.turnToHeading(-90, 1000);
        // chassis.moveToPose(10, -20, 270, 1500, {.minSpeed=70, .earlyExitRange=2});
        devices::scraper.extend();
        devices::splitter.extend();
        chassis.moveToPose(-10, -32.5, -90, 2000, {.minSpeed=70, .earlyExitRange=2});
        pros::delay(1500);
        chassis.moveToPose(-20, -32.5, -90, 2000, {.maxSpeed=55});
        pros::delay(1200);
        chassis.moveToPose(20.5, -34, -90, 2000, {.forwards=false, .maxSpeed=60});
        pros::delay(1000);
        keybindActions::intake::toggle_invert_mode(true);
        keybindActions::intake::set_load_speed(127);
        keybindActions::intake::load_bot();
    }
    void blueLeft() {}
    void blueRight() {}
}                                                