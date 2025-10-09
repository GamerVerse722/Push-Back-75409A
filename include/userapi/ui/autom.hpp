#pragma once

#include "gamers-forge/proslogger.hpp"
#include "liblvgl/misc/lv_color.h"

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

    // Only ment for internal api usage
    struct CallbackPassthrough {
        AutomMode mode;
        std::function<void()> callback;
    };

    inline PROSLogger::Logger log{"Autom Selector"};
    inline AutomMode selected_autom = NONE;
    extern std::function<void()> selected_callback;
    extern lv_obj_t* autom_screen;

    void initialize();
    void register_button(std::string text, int col, int row, lv_color_t bg_color, AutomMode mode, std::function<void()> callback);

    void run_automous();
}