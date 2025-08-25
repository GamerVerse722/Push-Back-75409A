#pragma once

#include "gamers-forge/proslogger.hpp"
#include "liblvgl/misc/lv_types.h"

namespace ui::op_control {
    void initialize();
    void image_render(lv_timer_t* timer);
    void debug_timer(lv_timer_t* timer);
    void logs(PROSLogger::LoggerEvent event);
    void init_keybinds();
    void create_category(std::string category);
    void create_row(std::string key, std::string description);
}