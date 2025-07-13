#include "userapi/configuration.hpp"
#include "controls/blocker.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "userapi/controls/doinker.hpp"
#include "userapi/controls/intake.hpp"

namespace devices {
    pros::MotorGroup right_motors({-1, 2, 3}, pros::MotorGearset::blue);
    pros::MotorGroup left_motors({4, -5, -6}, pros::MotorGearset::blue);

    pros::Imu imu(9);

    pros::adi::Encoder vertical_encoder('C', 'D');
    pros::adi::Encoder horizontal_encoder('A', 'B');

    lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -1.34375);
    lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, 4.5);

    lemlib::OdomSensors sensors(
        &vertical_tracking_wheel,
        nullptr,
        &horizontal_tracking_wheel,
        nullptr,
        &imu
    );

    // lateral PID controller
    // lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
    //     0, // integral gain (kI)
    //     3, // derivative gain (kD)
    //     3, // anti windup
    //     1, // small error range, in inches
    //     100, // small error range timeout, in milliseconds
    //     3, // large error range, in inches
    //     500, // large error range timeout, in milliseconds
    //     20 // maximum acceleration (slew)
    // );

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
    // lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
    //     0, // integral gain (kI)
    //     10, // derivative gain (kD)
    //     3, // anti windup
    //     1, // small error range, in degrees
    //     100, // small error range timeout, in milliseconds
    //     3, // large error range, in degrees
    //     500, // large error range timeout, in milliseconds
    //     0 // maximum acceleration (slew)
    // );

    lemlib::ControllerSettings angular_controller(4, // proportional gain (kP)
        0, // integral gain (kI)
        22, // derivative gain (kD)
        0, // anti windup
        0, // small error range, in degrees
        0, // small error range timeout, in milliseconds
        0, // large error range, in degrees
        0, // large error range timeout, in milliseconds
        0 // maximum acceleration (slew)
    );

    lemlib::Drivetrain drivetrain(&left_motors, // left motor group
        &right_motors, // right motor group
        14.5625, // 10 inch track width
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

    pros::adi::Pneumatics doinker(5, false);
    pros::adi::Pneumatics blocker(6, false);

    pros::Motor splitter(-7);
    pros::Motor intake(8);

    pros::Optical opticalSensor(11);
}

namespace controls {
    bmapping::ButtonHandler controlHandler(devices::controller);

    void configure() {
        using pros::controller_digital_e_t;
        using namespace keybindActions;

        // intake
        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_R1, {
            .onPress = intake::highGoalStorage,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_R1, {
            .onPress = intake::toggleHighGoal,
        });

        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_R2, {
            .onPress = intake::middleGoalOut,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_R2, {
            .onPress = intake::toggleMiddleGoal,
        });

        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_L1, {
            .onPress = intake::out,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_L1, {
            .onPress = intake::toggleOut,
        });

        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_L2, {
            .onPress = intake::intakeIn,
            .onRelease = intake::stop
        });

        controlHandler.registerKeybind(pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_L2, {
            .onPress = intake::toggleIntakeIn,
        });

        // doinker
        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_A, {
            .onPress = doinker::toggleDoinker,
        });

        // blocker
        controlHandler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_UP, {
            .onPress = blocker::toggleBlocker,
        });

        controlHandler.start();
    }
} 