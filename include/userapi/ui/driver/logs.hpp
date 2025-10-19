#pragma once

#include "gamers-forge/proslogger.hpp"
#include "liblvgl/misc/lv_types.h"

namespace ui::driver::logs {
    void initialize(lv_obj_t* parent);
    void logs(PROSLogger::LoggerEvent event);
}