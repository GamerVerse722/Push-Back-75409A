#pragma once

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"

#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/optical.hpp"

#include "gamers-forge/bmapper.hpp"

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

    extern pros::adi::Pneumatics scraper;
    extern pros::adi::Pneumatics splitter;

    extern pros::Motor intake;
    extern pros::Motor top_loader;
    extern pros::Motor bucket;

    extern pros::Optical opticalSensor;
}

namespace controls {
    extern BMapper::ButtonHandler button_handler;

    void configure();
}