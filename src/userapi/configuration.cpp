#include "userapi/configuration.hpp"

#include "userapi/controls/intake.hpp"
#include "userapi/controls/drive.hpp"

namespace devices {
    pros::MotorGroup right_motors({-1, 2, 3}, pros::MotorGearset::blue);
    pros::MotorGroup left_motors({4, -5, -6}, pros::MotorGearset::blue);

    pros::Imu imu(7);

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

    pros::adi::Pneumatics splitter(5, false);
    pros::adi::Pneumatics scraper(6, false);

    pros::Motor intake(8);
    pros::Motor top_loader(9);
    pros::Motor bucket(-10);

    pros::Optical opticalSensor(12);
}

namespace controls {
    BMapper::ButtonHandler button_handler(devices::controller);

    void configure() {
        using pros::controller_digital_e_t;
        using namespace keybindActions;

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_X)
            .setCategory("Drive")
            .onPress(drive::toggle_arcade);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2)
            .setCategory("Intake")
            .onPress(intake::bucket_out_high_score)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Intake")
            .onPress(intake::bucket_out_high_score);
            
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1)
            .setCategory("Intake")
            .onPress(intake::bucket_out_middle_score)
            .onRelease(intake::stop);
            
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Intake")
            .onPress(intake::bucket_out_middle_score);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L2)
            .setCategory("Intake")
            .onPress(intake::bucket_in)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L2, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Intake")
            .onPress(intake::bucket_in);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L1)
            .setCategory("Drive")
            .onPress(intake::bucket_out_lower_score)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_B)
            .setCategory("Drive")
            .onPress(intake::bucket_out_lower_score);
    }
} 