#include "userapi/ui/driver/debug.hpp"

#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/misc/lv_timer.h"

#include "lemlib/pose.hpp"

#include "userapi/ui/autom/autom_handler.hpp"
#include "userapi/handler/optical_normalize.hpp"
#include "userapi/configuration.hpp"

static lv_obj_t* labelDebug;

using namespace ui::autom;

namespace ui::driver::debug {
    void initialize(lv_obj_t* parent) {
        labelDebug = lv_label_create(parent);
        lv_timer_create(debug_timer, 50, nullptr);
    }

    void debug_timer(lv_timer_t* timer) {
        lemlib::Pose pose = devices::chassis.getPose();
        pros::c::optical_rgb_s_t normal_color = optical_normalize(devices::opticalSensor.get_rgb());

        std::string pos_str = std::format("X: {:.2f} Y: {:.2f} Theta: {:.2f}", pose.x, pose.y, pose.theta);

        std::string autom_mode_str = std::format("Mode: {0}, Color: {1}, Side: {2}", 
            handler::automModeToString(handler::active_mode),
            handler::automColorToString(handler::active_color),
            handler::automPositionToString(handler::active_position)
        );

        std::string color = std::format("Red: {:.2f}, Blue: {:.2f}, Distance: {}", normal_color.red, normal_color.blue, devices::opticalSensor.get_proximity());
        lv_label_set_text(labelDebug, std::format("{}\n{}\n{}", pos_str, autom_mode_str, color).c_str());
    }
}