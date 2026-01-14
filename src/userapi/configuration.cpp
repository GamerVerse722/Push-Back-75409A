#include "userapi/configuration.hpp"

#include "EZ-Template/drive/drive.hpp"
#include "EZ-Template/util.hpp"
#include "automonous.hpp"
#include "pros/misc.h"
#include "ui/autom/autom_handler.hpp"
#include "userapi/controls/intake.hpp"
#include "userapi/controls/drive.hpp"

#include "userapi/ui/autom/autom_handler.hpp"
#include "userapi/ui/autom/mode_selector.hpp"
#include "userapi/ui/autom/location_selector.hpp"

namespace devices {
    ez::Drive chassis(
        // These are your drive motors, the first motor is used for sensing!
        {5, -6, -8},     // Left Chassis Ports (negative port will reverse it!)
        {-2, 3, 4},  // Right Chassis Ports (negative port will reverse it!)

        9,      // IMU Port
        3.25,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
        360);   // Wheel RPM = cartridge * (motor gear / wheel gear)

    pros::adi::Pneumatics splitter(5, false);
    pros::adi::Pneumatics scraper(6, false);
    pros::adi::Pneumatics descore(8, false);

    pros::MotorGroup loader_motors({-18, 19});

    pros::Motor top_loader(15);

    pros::Optical opticalSensor(11);
}

namespace configuration::drive {
    using namespace devices;

    void default_constants() {
        // P, I, D, and Start I
        // chassis.pid_drive_constants_set(20.0, 0.0, 100.0);         // fwd/rev constants, used for odom and non odom motions
        chassis.pid_drive_constants_forward_set(18.0, 0.0, 100.0);
        chassis.pid_drive_constants_backward_set(20.0, 0.0, 100.0);

        chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Holds the robot straight while going forward without odom
        chassis.pid_turn_constants_set(4.0, 0.05, 21.0, 15.0);     // Turn in place constants
        chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
        chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
        chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

        // Exit conditions
        chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
        chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
        chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
        chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
        chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
        chassis.pid_turn_chain_constant_set(3_deg);
        chassis.pid_swing_chain_constant_set(5_deg);
        chassis.pid_drive_chain_constant_set(3_in);

        // Slew constants
        chassis.slew_turn_constants_set(3_deg, 70);
        chassis.slew_drive_constants_set(3_in, 70);
        chassis.slew_swing_constants_set(3_in, 80);

        // The amount that turns are prioritized over driving in odom motions
        // - if you have tracking wheels, you can run this higher.  1.0 is the max
        chassis.odom_turn_bias_set(0.9);

        chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
        chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
        chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

        chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
    }

    void initialize() {
        // Configure your chassis controls
        chassis.opcontrol_curve_buttons_toggle(true);   // Enables modifying the controller curve with buttons on the joysticks
        chassis.opcontrol_drive_activebrake_set(0.0);   // Sets the active brake kP. We recommend ~2.  0 will disable.
        chassis.opcontrol_curve_default_set(0.0, 0.0);  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

        default_constants();

        chassis.initialize();
        master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
    }
}

namespace configuration::controls {
    BMapper::ButtonHandler button_handler(master);

    void configure() {
        using pros::controller_digital_e_t;
        using namespace keybindActions;

        // Drive
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_A, pros::E_CONTROLLER_DIGITAL_Y)
            .setCategory("Drive")
            .onPress(keybindActions::drive::toggle_arcade);

        // Scrapper
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_B)
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
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_Y)
            .setCategory("Load")
            .onPress(intake::load_bot);

        // Score
        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1)
            .setCategory("Score")
            .onPress(intake::score_low)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_Y)
            .setCategory("Score")
            .onPress(intake::score_low);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2)
            .setCategory("Score")
            .onPress(intake::score_high)
            .onRelease(intake::stop);

        button_handler.bind(pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_Y)
            .setCategory("Score")
            .onPress(intake::score_high);
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

        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::RED, AutomPosition::LEFT, autom::Eliminations::left);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::RED, AutomPosition::RIGHT, autom::Eliminations::right);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::BLUE, AutomPosition::LEFT, autom::Eliminations::left);
        handler::register_autom(AutomMode::ELIMINATIONS, AutomColor::BLUE, AutomPosition::RIGHT, autom::Eliminations::right);

        handler::register_autom(AutomMode::SKILLS, AutomColor::COLOR_NONE, AutomPosition::NO_POSITION, nullptr);
    }
}