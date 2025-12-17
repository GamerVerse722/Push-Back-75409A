#pragma once

#include "userapi/ui/autom/autom_handler.hpp"

using namespace ui::autom::handler;

namespace keybindActions::intake {
    bool valid_ball();
    void load_bypass();
    void toggle_invert_mode(bool enabled);
    AutomColor invert_color(AutomColor color);

    void park_activation();
    void park();
    
    void load_bot();
    void score_high();
    void score_low();
    
    void stop();

    void set_load_speed(int speed);
    void reset_default_load_speed();
}