#include "userapi/automonous.hpp"

#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;

namespace autom {
    void skills() {}
    void redLeft() {
        // chassis.setPose(-48.5, 16.5, 90);
        // keybindActions::intake::load_bot();
        // chassis.moveToPose(-20, 22, 70, 3000);
        // chassis.turnToHeading(315, 2000);
        // pros::delay(500);
        // chassis.moveToPose(-40, 8, -45, 3000, {.forwards=true});
        // pros::delay(2000);
        // keybindActions::intake::score_middle();

        // chassis.setPose(0, 0, 90);
        // keybindActions::intake::load_bot();
        // chassis.moveToPose(18, 7, 45, 2000);
        // chassis.turnToHeading(-45, 2000);
        // chassis.moveToPose(27, 0, -45, 2000, {.forwards=false});
        // pros::delay(100);
        // keybindActions::intake::score_middle();

        chassis.setPose(0, 0, 90);
        keybindActions::intake::load_bot();
        chassis.moveToPose(28, 12, 45, 2000);
        chassis.turnToHeading(-45, 2000);
        chassis.moveToPose(38, -4, -45, 2000, {.forwards=false});
        pros::delay(800);
        keybindActions::intake::score_middle();
        pros::delay(1500);
        chassis.moveToPose(-10, 30, -90, 3000);
    }
    void redRight() {}
    void blueLeft() {}
    void blueRight() {}
}