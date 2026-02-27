#include "userapi/controls/intake.hpp"

#include "userapi/configuration.hpp"

using namespace devices;

static const int DEFUALT_LOW_SCORE_SPEED = -127;
static int LOW_SCORE_SPEED = -127;

namespace keybindActions::intake {
    void set_low_score_speed(int speed) {
        LOW_SCORE_SPEED = speed;
    }

    void reset_low_score_speed() {
        LOW_SCORE_SPEED = DEFUALT_LOW_SCORE_SPEED;
    }

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
        loader.move(LOW_SCORE_SPEED);
    }

    void stop() {
        hardstop.retract();
        loader.move(0);
    }
}