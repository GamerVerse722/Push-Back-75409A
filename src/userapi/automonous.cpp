#include "userapi/automonous.hpp"

// #include "lemlib/pose.hpp"
#include "pros/rtos.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"

using namespace devices;

namespace autom::Qualifications {
    void left() {
        chassis.setPose(0, 0, 0);
        chassis.moveToPoint(0, 24, 10000);
        // chassis.moveToPose(0, 24, 0, 10000);
    }
    void right() {
        chassis.setPose(0, 0, 0);

    }
}

namespace autom::Eliminations {
    void left() {
        chassis.setPose(0, 0, 0);

    }
    void right() {
        chassis.setPose(0, 0, 0);

    }
}