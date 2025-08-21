#include "drive.hpp"

namespace keybindActions::drive {
    bool arcade = false;

    void toggle_arcade() {
        if (arcade) {
            arcade = false;
        } else {
            arcade = true;
        }
    }

    bool is_arcade() {
        return arcade;
    }
}