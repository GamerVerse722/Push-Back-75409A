#include "userapi/ui/driver/debug.hpp"

#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/misc/lv_timer.h"

#include "lemlib/pose.hpp"

#include "userapi/handler/optical_normalize.hpp"
#include "userapi/configuration.hpp"
#include "userapi/ui/autom.hpp"

static lv_obj_t* labelDebug;

namespace ui::driver::debug {
    void initialize(lv_obj_t* parent) {
        labelDebug = lv_label_create(parent);
        lv_timer_create(debug_timer, 50, nullptr);
    }

    void debug_timer(lv_timer_t* timer) {
        lemlib::Pose pose = devices::chassis.getPose();
        pros::c::optical_rgb_s_t normal_color = optical_normalize(devices::opticalSensor.get_rgb());

        std::string pos_str = std::format("X: {:.2f} Y: {:.2f} Theta: {:.2f}", pose.x, pose.y, pose.theta);
        std::string autom_mode_str = std::format("Automonous Mode: {}", ui::autom_selector::automModeToString(ui::autom_selector::selected_autom));
        std::string color = std::format("Red: {:.2f}, Blue: {:.2f}, Distance: {}", normal_color.red, normal_color.blue, devices::opticalSensor.get_proximity());
        std::string park = std::format("Park Distance: {}", devices::distanceSensor.get_distance());
        lv_label_set_text(labelDebug, std::format("{}\n{}\n{}\n{}", pos_str, autom_mode_str, color, park).c_str());
    }
}