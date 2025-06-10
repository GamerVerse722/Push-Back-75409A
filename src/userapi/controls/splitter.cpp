#include "userapi/controls/splitter.hpp"
#include "userapi/configuration.hpp"

namespace keybindActions::splitter {
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
        devices::splitter.move(127);
    }

    void out() {
        resetToggle();
        devices::splitter.move(-127);
    }

    void stop() {
        resetToggle();
        devices::splitter.move(0);
    }

    void resetToggle() {
        toggleInEnabled = false;
        toggleOutEnabled = false;
    }
}