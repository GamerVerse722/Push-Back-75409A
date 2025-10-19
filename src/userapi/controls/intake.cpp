#include "userapi/controls/intake.hpp"

#include "pros/optical.h"
#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/handler/optical_normalize.hpp"
#include "userapi/ui/autom.hpp"

using namespace devices;

static bool color_sort_enabled = false;
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

        if (ui::autom_selector::selected_color == ui::autom_selector::AutomColor::RED && color.red > color.blue) {return true;}
        else if (ui::autom_selector::selected_color == ui::autom_selector::AutomColor::BLUE && color.red < color.blue) {return true;}
        else if (ui::autom_selector::selected_color == ui::autom_selector::AutomColor::COLOR_NONE) {return true;}
        return false;
    }

    void load_bypass() {
        if (devices::opticalSensor.get_proximity() < 100) {devices::top_loader.move(50); return;}

        if (!valid_ball()) {devices::top_loader.move(50);return;}

        devices::top_loader.move(0);
    }

    void load_bot() {
        color_sort_enabled = true;
        keybindActions::intake::load_bypass();
        devices::intake.move(127);
        devices::lift.move(127);
    }

    void park_activation() {
        if (devices::distanceSensor.get_distance() < 120) {
            devices::top_loader.move(0);
            devices::intake.move(0);
            devices::lift.move(0);
            // devices::park.extend();
            devices::controller.rumble("--");
        }
    }

    void park() {
        devices::top_loader.move(-127);
        devices::intake.move(-127);
        devices::lift.move(-50);
        color_sort_enabled = false;
    }

    void score_high() {
        // devices::splitter.extend();
        devices::top_loader.move(127);
        devices::intake.move(127);
        devices::lift.move(127);
        color_sort_enabled = false;
    }

    void score_middle() {
        // devices::splitter.retract();
        devices::top_loader.move(127);
        devices::intake.move(127);
        devices::lift.move(127);
        color_sort_enabled = false;
    }

    void score_low() {
        // devices::splitter.retract();
        devices::top_loader.move(-127);
        devices::intake.move(-127);
        devices::lift.move(-127);
        color_sort_enabled = false;
    }

    void stop() {
        devices::top_loader.move(0);
        devices::intake.move(0);
        devices::lift.move(0);
        color_sort_enabled = false;
    }
}