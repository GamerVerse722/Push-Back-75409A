#include "userapi/configuration.hpp"

#include "pros/adi.hpp"
#include "pros/misc.h"
#include "userapi/controls/intake.hpp"
#include "userapi/controls/drive.hpp"

namespace devices {
    pros::MotorGroup right_motors({-2, 3, 4}, pros::MotorGearset::blue);
    pros::MotorGroup left_motors({5, -6, -8}, pros::MotorGearset::blue);

    pros::Imu imu(8);

    pros::adi::Encoder vertical_encoder('A', 'B', true);
    pros::adi::Encoder horizontal_encoder('C', 'D');

    lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -0.78125);
    lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -4.25);

    // lemlib::OdomSensors sensors(
    //     &vertical_tracking_wheel,
    //     nullptr,
    //     &horizontal_tracking_wheel,
    //     nullptr,
    //     &imu
    // );

    lemlib::OdomSensors sensors(
        nullptr,
        nullptr,
        nullptr,
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
    lemlib::ControllerSettings lateral_controller(12, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              35, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
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
    lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
    );

    lemlib::Drivetrain drivetrain(&left_motors, // left motor group
        &right_motors, // right motor group
        11.9375, // 10 inch track width
        lemlib::Omniwheel::NEW_325, // using new 4" omnis
        360, // drivetrain rpm is 360
        8 // horizontal drift is 2 (for now)
    );

    lemlib::Chassis chassis(drivetrain, // drivetrain settings
        lateral_controller, // lateral PID settings
        angular_controller, // angular PID settings
        sensors // odometry sensors
    );

    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    pros::adi::Pneumatics splitter(5, false);
    pros::adi::Pneumatics scraper(6, false);
    pros::adi::Pneumatics descore(8, false);

    pros::Motor top_loader(15);
    pros::Motor intake(19);
    pros::Motor lift(-18); // Unused

    pros::Optical opticalSensor(11);
}

namespace controls {
    BMapper::ButtonHandler button_handler(devices::controller);

    void configure() {
        using pros::controller_digital_e_t;
        using namespace keybindActions;

        // Drive
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_A, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Drive")
            .onPress(drive::toggle_arcade);

        // Scrapper
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_UP)
            .setCategory("Scrapper")
            .onPress([]() -> void {
                devices::scraper.toggle();
            });

        // Splitter
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_X)
            .setCategory("Splitter")
            .onPress([]() -> void {
                devices::splitter.toggle();
            });

        // Descore
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_DOWN)
            .setCategory("Descore")
            .onPress([]() -> void {
                devices::descore.toggle();
            });


        // Load
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L1)
            .setCategory("Load")
            .onPress(intake::load_bot)
            // .onHold(intake::load_bypass)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Load")
            .onPress(intake::load_bot);

        // Score
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1)
            .setCategory("Score")
            .onPress(intake::score_low)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Score")
            .onPress(intake::score_low);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2)
            .setCategory("Score")
            .onPress(intake::score_high)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Score")
            .onPress(intake::score_high);
    }
} 