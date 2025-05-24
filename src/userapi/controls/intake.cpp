#include "userapi/controls/intake.hpp"
#include "userapi/configuration.hpp"

namespace keybindActions::intake {
    void toggleIn() {
        if (toggleInEnabled) {
            in();
            resetToggle();

        } else {
            stop();
            toggleInEnabled = true;
            toggleOutEnabled = false;

        }
    }
    void toggleOut() {
        if (toggleOutEnabled) {
            out();
            resetToggle();

        } else {
            stop();
            toggleInEnabled = false;
            toggleOutEnabled = true;

        }
    }

    void in() {
        resetToggle();
        devices::intake.move(127);
    }

    void out() {
        resetToggle();
        devices::intake.move(-127);
    }

    void stop() {
        resetToggle();
        devices::intake.move(0);
    }

    void resetToggle() {
        toggleInEnabled = false;
        toggleOutEnabled = false;
    }
}