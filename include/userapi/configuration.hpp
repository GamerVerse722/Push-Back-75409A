#pragma once

#include "lemlib/chassis/chassis.hpp"

#include "gamers-forge/bmapper.hpp"

#include "pros/motor_group.hpp"
#include "pros/optical.hpp"
#include "pros/misc.hpp"
#include "pros/adi.hpp"


namespace devices{
    extern pros::MotorGroup left_motors;
    extern pros::MotorGroup right_motors;

    extern pros::Imu imu;

    extern lemlib::OdomSensors sensors;

    extern lemlib::ControllerSettings lateral_controller;
    extern lemlib::ControllerSettings angular_controller;

    extern lemlib::Drivetrain drivetrain;

    extern lemlib::Chassis chassis;

    extern pros::Controller controller;

    extern pros::adi::Pneumatics splitter;
    extern pros::adi::Pneumatics scraper;
    extern pros::adi::Pneumatics descore;

    extern pros::MotorGroup loader_motors;
    extern pros::Motor top_loader;

    extern pros::Optical opticalSensor;
}

namespace configuration::controls {
    extern BMapper::ButtonHandler button_handler;

    void configure();
}

namespace configuration::autonomous {
    void configure();
}