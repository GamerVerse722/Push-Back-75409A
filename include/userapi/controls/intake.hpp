#pragma once

#include "userapi/ui/autom.hpp"

namespace keybindActions::intake {
    bool valid_ball();
    void load_bypass();
    void toggle_invert_mode(bool enabled);
    ui::autom_selector::AutomColor invert_color(ui::autom_selector::AutomColor color);

    void park_activation();
    void park();
    
    void load_bot();
    void score_high();
    void score_low();
    
    void stop();

    void set_load_speed(int speed);
    void reset_default_load_speed();
}