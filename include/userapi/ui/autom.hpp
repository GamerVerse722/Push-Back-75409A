#pragma once

#include "gamers-forge/proslogger.hpp"

#include "liblvgl/misc/lv_types.h"

#include <functional>
#include <string>
#include <map>

namespace ui::autom_selector {
    enum AutomMode {
        RED_LEFT = 0,
        RED_RIGHT = 1,
        BLUE_LEFT = 2,
        BLUE_RIGHT = 3,
        SKILLS = 4,
        NONE = 5,
    };

    enum AutomColor {
        RED = 0,
        BLUE = 1,
        COLOR_NONE = 2,
    };

    inline std::string automModeToString(AutomMode mode) {
        switch (mode) {
            case RED_LEFT: return "Red Left";
            case RED_RIGHT: return "Red Right";
            case BLUE_LEFT: return "Blue Left";
            case BLUE_RIGHT: return "Blue Right";
            case SKILLS: return "Skills";
            case NONE: return "None";
            default: return "Unknown";
        }
    }

    typedef std::function<void()> callback_method;

    inline PROSLogger::Logger log{"Autom Selector"};
    inline AutomMode selected_autom = AutomMode::NONE;
    inline AutomColor selected_color = AutomColor::COLOR_NONE;
    extern std::map<AutomMode, callback_method> callback_map;
    extern lv_obj_t* autom_screen;

    void initialize();
    void register_button(std::string text, int col, int row, AutomMode mode, AutomColor color, callback_method callback);

    void run_automous();
    void invert_selected_color();
}