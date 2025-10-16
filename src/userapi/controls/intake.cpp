#include "userapi/controls/intake.hpp"

#include "userapi/configuration.hpp"

using namespace devices;

namespace keybindActions::intake {
    void load_bot() {
        devices::top_loader.move(0);
        devices::intake.move(127);
        devices::lift.move(127);
    }

    void score_high() {
        devices::splitter.extend();
        devices::top_loader.move(127);
        devices::intake.move(127);
        devices::lift.move(127);
    }

    void score_middle() {
        devices::splitter.retract();
        devices::top_loader.move(127);
        devices::intake.move(127);
        devices::lift.move(127);
    }

    void score_low() {
        devices::splitter.retract();
        devices::top_loader.move(-127);
        devices::intake.move(-127);
        devices::lift.move(-127);
    }

    void stop() {
        devices::top_loader.move(0);
        devices::intake.move(0);
        devices::lift.move(0);
    }
}