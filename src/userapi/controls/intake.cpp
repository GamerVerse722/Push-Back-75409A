#include "userapi/controls/intake.hpp"
#include "userapi/configuration.hpp"

namespace keybindActions::intake {
    void toggleHighGoal() {
        if (toggleHighGoalEnabled) {
            stop();
        } else {
            highGoalStorage();
            toggleHighGoalEnabled = true;
        }
    }
    void toggleMiddleGoal() {
        if (toggleMiddleGoalEnabled) {
            stop();
        } else {
            middleGoalOut();
            toggleMiddleGoalEnabled = true;
        }
    }
    void toggleOut() {
        if (toggleOutEnabled) {
            stop();
        } else {
            out();
            toggleOutEnabled = true;
        }
    }

    void highGoalStorage() {
        resetToggle();
        devices::intake.move(127);
        devices::splitter.move(127);
    }
    void middleGoalOut() {
        resetToggle();
        devices::intake.move(127);
        devices::splitter.move(-127);
    }
    void out() {
        resetToggle();
        devices::intake.move(-127);
        devices::splitter.move(-127);
    }

    void stop() {
        resetToggle();
        devices::intake.move(0);
        devices::splitter.move(0);
    }

    void resetToggle() {
        toggleHighGoalEnabled = false;
        toggleMiddleGoalEnabled = false;
        toggleOutEnabled = false;
    }
}