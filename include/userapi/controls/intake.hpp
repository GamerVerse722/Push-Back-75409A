#pragma once

namespace keybindActions::intake {
    void set_low_score_speed(int speed);
    void reset_low_score_speed();
    
    void load_bot();
    void score_high();
    void score_low();
    
    void stop();
}