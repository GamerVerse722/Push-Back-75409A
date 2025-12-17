#pragma once

#include <functional>
#include <string_view>

namespace ui::autom::handler {
    enum AutomMode {
        QUALIFICATIONS = 0,
        ELIMINATIONS = 1,
        SKILLS = 2,
        NONE = 3
    };
    
    enum AutomPosition {
        LEFT = 0,
        RIGHT = 1,
        NO_POSITION = 2
    };

    enum AutomColor {
        RED = 0,
        BLUE = 1,
        COLOR_NONE = 2
    };
    
    enum AutomSideColor {
        RED_LEFT = 0,
        RED_RIGHT = 1,
        BLUE_LEFT = 2,
        BLUE_RIGHT = 3,
        NO_COLOR_AND_POSITION = 4,
    };

    using callback_method = std::function<void()>;

    extern AutomMode active_mode;
    extern AutomSideColor active_side;
    extern AutomPosition active_position;
    extern AutomColor active_color;
    extern callback_method active_callback;

    // Helper Methods
    AutomSideColor   get_side_color(AutomSideColor color, AutomPosition pos);
    AutomPosition    get_position_from_side_color(AutomSideColor sc);
    AutomColor       get_color_from_side_color(AutomSideColor sc);
    std::string_view automModeToString(AutomMode mode);
    std::string_view automColorToString(AutomColor color);
    std::string_view automPositionToString(AutomPosition pos);
    std::string_view automPositionColorToString(AutomSideColor sc);

    
    void register_callback_method(AutomMode mode, AutomColor color, AutomPosition position, callback_method callback);

    void select_autom(AutomMode mode, AutomSideColor side_color);

    void run_automous();
}