#include "userapi/controls/blocker.hpp"
#include "userapi/configuration.hpp"

namespace keybindActions::blocker{
    void toggleBlocker() {
        devices::blocker.toggle();
    }
}