#pragma once

// #include "lemlib/chassis/chassis.hpp"

#include "EZ-Template/drive/drive.hpp"

#include "gamers-forge/bmapper.hpp"

// #include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/optical.hpp"
#include "pros/adi.hpp"


namespace devices {
    extern ez::Drive chassis;
    
    extern pros::adi::Pneumatics splitter;
    extern pros::adi::Pneumatics scraper;
    extern pros::adi::Pneumatics descore;

    extern pros::MotorGroup loader_motors;
    extern pros::Motor top_loader;

    extern pros::Optical opticalSensor;
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