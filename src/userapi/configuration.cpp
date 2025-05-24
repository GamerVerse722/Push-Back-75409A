#include "userapi/configuration.hpp"
#include "pros/misc.h"
#include "userapi/controls/intake.hpp"

namespace devices {
    pros::MotorGroup right_motors({-1, 2, 3}, pros::MotorGearset::blue);
    pros::MotorGroup left_motors({4, -5, -6}, pros::MotorGearset::blue);

    pros::Imu imu(7);

    pros::adi::Encoder vertial_encoder('A', 'B');
    pros::adi::Encoder horizontal_encoder('C', 'D');

    lemlib::TrackingWheel vertical_tracking_wheel(&vertial_encoder, lemlib::Omniwheel::NEW_275, -1);
    lemlib::TrackingWheel horizontal_tracking_wheel(&vertial_encoder, lemlib::Omniwheel::NEW_275, -2);

    lemlib::OdomSensors sensors(
        &vertical_tracking_wheel,
        nullptr,
        &horizontal_tracking_wheel,
        nullptr,
        &imu
    );

    // lateral PID controller
    lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
        0, // integral gain (kI)
        3, // derivative gain (kD)
        3, // anti windup
        1, // small error range, in inches
        100, // small error range timeout, in milliseconds
        3, // large error range, in inches
        500, // large error range timeout, in milliseconds
        20 // maximum acceleration (slew)
    );
    // angular PID controller
    lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
        0, // integral gain (kI)
        10, // derivative gain (kD)
        3, // anti windup
        1, // small error range, in degrees
        100, // small error range timeout, in milliseconds
        3, // large error range, in degrees
        500, // large error range timeout, in milliseconds
        0 // maximum acceleration (slew)
    );

    lemlib::Drivetrain drivetrain(&left_motors, // left motor group
        &right_motors, // right motor group
        10, // 10 inch track width
        lemlib::Omniwheel::NEW_325, // using new 4" omnis
        360, // drivetrain rpm is 360
        2 // horizontal drift is 2 (for now)
    );

    lemlib::Chassis chassis(drivetrain, // drivetrain settings
        lateral_controller, // lateral PID settings
        angular_controller, // angular PID settings
        sensors // odometry sensors
    );

    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    pros::Motor intake(8);
}

namespace controls {
    bmapping::ButtonHandler controlHandler(devices::controller);

    void configure() {
        using pros::controller_digital_e_t;
        using namespace keybindActions;

        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_R1, {
            .onPress = intake::out,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_R1, {
            .onPress = intake::toggleOut
        });

        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_R2, {
            .onPress = intake::in,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_R2, {
            .onPress = intake::toggleIn
        });

        controlHandler.start();
    }
}