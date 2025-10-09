#pragma once

#include "gamers-forge/proslogger.hpp"

#include <string>

namespace ui::autom_selector {
    enum AutomMode {
        RED_LEFT = 0,
        RED_RIGHT = 1,
        BLUE_LEFT = 2,
        BLUE_RIGTH = 3,
        SKILLS = 4,
        NONE = 5,
    };

    inline std::string automModeToString(AutomMode mode) {
        switch (mode) {
            case RED_LEFT: return "Red Left";
            case RED_RIGHT: return "Red Right";
            case BLUE_LEFT: return "Blue Left";
            case BLUE_RIGTH: return "Blue Right";
            case SKILLS: return "Skills";
            case NONE: return "None";
            default: return "Unknown";
        }
    }

    inline PROSLogger::Logger log{"345"};

    void initialize();
    void register_button(std::string text, int collum, int row);
    void register_autom(AutomMode mode, std::function<void()> method);
}