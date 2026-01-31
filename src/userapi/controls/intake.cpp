#include "userapi/controls/intake.hpp"

#include "userapi/configuration.hpp"

using namespace devices;


namespace keybindActions::intake {
    void load_bot() {
        hardstop.retract();
        loader.move(127);
    }

    void score_high() {
        hardstop.extend();
        loader.move(127);
    }

    void score_low() {
        hardstop.retract();
        loader.move(-127);
    }

    void stop() {
        hardstop.retract();
        loader.move(0);
    }
}