#pragma once

#include "EZ-Template/drive/drive.hpp"

#include "gamers-forge/bmapper.hpp"

#include "pros/motor_group.hpp"
#include "pros/adi.hpp"


namespace devices {
    extern ez::Drive chassis;
    
    extern pros::adi::Pneumatics splitter;
    extern pros::adi::Pneumatics scraper;
    extern pros::adi::Pneumatics descore;
    extern pros::adi::Pneumatics hardstop;

    extern pros::MotorGroup loader;
}

namespace configuration::drive {
    void default_constants();
    void initialize();
}

namespace configuration::controls {
    extern BMapper::ButtonHandler button_handler;

    void configure();
}

namespace configuration::autonomous {
    void configure();
}