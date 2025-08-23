#include "userapi/controls/intake.hpp"

#include "userapi/configuration.hpp"

using namespace devices;

namespace keybindActions::intake {
    void bucket_in() {
        devices::intake.move(127);
        devices::bucket.move(0);
        devices::top_loader.move(127);
        devices::splitter.extend();
    }

    void bucket_out_lower_score() {
        devices::intake.move(-127);
        devices::bucket.move(127);
        devices::top_loader.move(0);
    }

    void bucket_out_middle_score() {
        devices::intake.move(127);
        devices::bucket.move(127);
        devices::top_loader.move(-127);
    }

    void bucket_out_high_score() {
        devices::intake.move(127);
        devices::bucket.move(127);
        devices::top_loader.move(127);
        devices::splitter.retract();
    }

    void stop() {
        devices::intake.move(0);
        devices::top_loader.move(0);
        devices::bucket.move(0);
    }
}