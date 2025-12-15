#pragma once

#include "liblvgl/core/lv_obj.h"
#include "liblvgl/misc/lv_types.h"

#include "userapi/ui/autom/autom_handler.hpp"


using namespace ui::autom::handler;
namespace ui::autom::location_selector {
    inline lv_obj_t* selector = lv_obj_create(NULL);
    inline AutomMode active_mode = AutomMode::NONE;

    void initialize();
}