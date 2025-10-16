#pragma once

#include "lemlib/chassis/trackingWheel.hpp"
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

    extern pros::adi::Encoder vertical_encoder;
    extern pros::adi::Encoder horizontal_encoder;

    extern lemlib::TrackingWheel vertical_tracking_wheel;
    extern lemlib::TrackingWheel horizontal_tracking_wheel;

    extern lemlib::OdomSensors sensors;

    extern lemlib::ControllerSettings lateral_controller;
    extern lemlib::ControllerSettings angular_controller;

    extern lemlib::Drivetrain drivetrain;

    extern lemlib::Chassis chassis;

    extern pros::Controller controller;

    extern pros::adi::Pneumatics splitter;
    extern pros::adi::Pneumatics scraper;

    extern pros::Motor top_loader;
    extern pros::Motor intake;
    extern pros::Motor lift;

    // extern pros::Motor test1;
    // extern pros::Motor test2;
    // extern pros::Motor test3;
    // extern pros::Motor test4;

    extern pros::Optical opticalSensor;
}

namespace controls {
    extern BMapper::ButtonHandler button_handler;

    void configure();
}