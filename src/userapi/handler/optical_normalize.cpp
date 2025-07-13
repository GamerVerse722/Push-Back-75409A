#include "userapi/handler/optical_normalize.hpp"

pros::c::optical_rgb_s_t optical_normalize(pros::c::optical_rgb_s_t color) {
    float total = color.red + color.green + color.blue;

    return {
        color.red / total,
        color.green / total,
        color.blue / total,
        (color.red + color.green + color.blue) / 3.0
    };
}