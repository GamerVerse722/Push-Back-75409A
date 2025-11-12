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
        chassis.moveToPose(35, -5, -45, 1500, {.forwards=false});
        pros::delay(1000);
        keybindActions::intake::score_middle();
        pros::delay(1800);
        keybindActions::intake::load_bot();

        // Grab Score
        // chassis.moveToPose(0, 23, -60, 2000, {.minSpeed=70, .earlyExitRange=4});
        devices::scraper.extend();
        chassis.moveToPose(-22, 30, -90, 2500, {.minSpeed=75});
        // chassis.moveToPose(-15, 40, -90, 1000, {.minSpeed=100});

    }
    void redRight() {}
    void blueLeft() {}
    void blueRight() {}
}