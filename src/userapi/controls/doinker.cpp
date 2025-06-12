#include "userapi/controls/doinker.hpp"
#include "userapi/configuration.hpp"

namespace keybindActions::doinker {
    void toggleDoinker() {
        devices::doinker.toggle();
    }
}