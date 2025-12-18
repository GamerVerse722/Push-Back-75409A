#include "userapi/controls/intake.hpp"

#include "pros/optical.h"
#include "pros/rtos.hpp"

#include "userapi/handler/optical_normalize.hpp"
#include "userapi/configuration.hpp"
#include "userapi/ui/autom/autom_handler.hpp"

using namespace devices;
using namespace ui::autom::handler;

static bool color_sort_enabled = false;
static bool invert_mode_enabled = false;
static int default_load_speed = 50;
static int load_speed = default_load_speed;

static pros::Task color_sort([](){
    while (true) {
        if (color_sort_enabled) {
            keybindActions::intake::load_bypass();
        }
        pros::delay(10);
    }
});

namespace keybindActions::intake {
    bool valid_ball() {
        pros::c::optical_rgb_s_t color = optical_normalize(devices::opticalSensor.get_rgb());

        AutomColor local_color = current_color;

        if (invert_mode_enabled) {
            local_color = invert_color(local_color);
        }

        if (local_color == AutomColor::RED && color.red > color.blue) {return true;}
        else if (local_color == AutomColor::BLUE && color.red < color.blue) {return true;}
        else if (local_color == AutomColor::COLOR_NONE) {return true;}
        return false;
    }

    void load_bypass() {
        if (devices::opticalSensor.get_proximity() < 100) {devices::top_loader.move(load_speed); return;}

        if (!valid_ball()) {devices::top_loader.move(load_speed);return;}

        devices::top_loader.move(0);
    }

    void toggle_invert_mode(bool enabled) {
        invert_mode_enabled = enabled;
    }

    AutomColor invert_color(AutomColor color) {
        switch (color) {
            case AutomColor::RED: return AutomColor::BLUE;
            case AutomColor::BLUE: return AutomColor::RED;
            case AutomColor::COLOR_NONE: return AutomColor::COLOR_NONE;
            default: return AutomColor::COLOR_NONE;
        }
    }

    void load_bot() {
        color_sort_enabled = true;
        keybindActions::intake::load_bypass();
        devices::loader_motors.move(127);
    }

    void score_high() {
        devices::top_loader.move(127);
        devices::loader_motors.move(127);
        color_sort_enabled = false;
    }

    void score_low() {
        devices::top_loader.move(-127);
        devices::loader_motors.move(-127);
        color_sort_enabled = false;
    }

    void stop() {
        devices::top_loader.move(0);
        devices::loader_motors.move(0);
        color_sort_enabled = false;
    }

    void set_load_speed(int speed) {
        load_speed = speed;
    }

    void reset_default_load_speed() {
        load_speed = default_load_speed;
    }
}