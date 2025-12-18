#pragma once

#include "liblvgl/core/lv_obj.h"
#include "liblvgl/misc/lv_types.h"

#include "userapi/ui/autom/autom_handler.hpp"

using namespace ui::autom::handler;

namespace ui::autom::mode_selector {
    inline lv_obj_t* mode_screen = lv_obj_create(NULL);

    void initialize();

    void register_button(AutomMode color, int row);

    void button_event_handler(lv_event_t* e);
}