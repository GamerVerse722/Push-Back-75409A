#include "userapi/configuration.hpp"

#include "automonous.hpp"
#include "pros/misc.h"
#include "ui/autom/autom_handler.hpp"
#include "userapi/controls/intake.hpp"
#include "userapi/controls/drive.hpp"

#include "userapi/ui/autom/autom_handler.hpp"
#include "userapi/ui/autom/mode_selector.hpp"
#include "userapi/ui/autom/location_selector.hpp"

namespace devices {
    pros::MotorGroup right_motors({-2, 3, 4}, pros::MotorGearset::blue);
    pros::MotorGroup left_motors({5, -6, -8}, pros::MotorGearset::blue);

    pros::Imu imu(9);

    lemlib::OdomSensors sensors(
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &imu
    );

    // lateral PID controller
    lemlib::ControllerSettings lateral_controller(12, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              15, // derivative gain (kD)
                                              0, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
    );


    // angular PID controller
    lemlib::ControllerSettings angular_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              55, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
    );

    lemlib::Drivetrain drivetrain(&left_motors, // left motor group
        &right_motors, // right motor group
        12, // 10 inch track width
        lemlib::Omniwheel::NEW_325, // using new 4" omnis
        505, // drivetrain rpm is 360
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

    pros::MotorGroup loader_motors({-18, 19});

    pros::Motor top_loader(15);

    pros::Optical opticalSensor(11);
}

namespace configuration::controls {
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

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_LEFT)
            .setCategory("Testing")
            .onPress([&](){
                run_autom();
            });
    }
} 

namespace configuration::autonomous {
    void configure() {
        using namespace ui::autom;

        location_selector::initialize();
        mode_selector::initialize();

        handler::register_autom(AutomMode::QUALIFICATIONS, AutomColor::RED, AutomPosition::LEFT, autom::Qualifications::left);
        handler::register_autom(AutomMode::QUALIFICATIONS, AutomColor::RED, AutomPosition::RIGHT, autom::Qualifications::right);
        handler::register_autom(AutomMode::QUALIFICATIONS, AutomColor::BLUE, AutomPosition::LEFT, autom::Qualifications::left);
        handler::register_autom(AutomMode::QUALIFICATIONS, AutomColor::BLUE, AutomPosition::RIGHT, autom::Qualifications::right);

        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::RED, AutomPosition::LEFT, nullptr);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::RED, AutomPosition::RIGHT, nullptr);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::BLUE, AutomPosition::LEFT, nullptr);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::BLUE, AutomPosition::RIGHT, nullptr);

        handler::register_autom(AutomMode::SKILLS, AutomColor::COLOR_NONE, AutomPosition::NO_POSITION, nullptr);
    }
}