#include "main.h"

#include "gamers-forge/proslogger.hpp"

#include "liblvgl/display/lv_display.h"
#include "pros/rtos.hpp"
#include "pros/misc.h"

#include "userapi/configuration.hpp"
#include "userapi/controls/drive.hpp"
#include "userapi/ui/autom.hpp"
#include "userapi/ui/op_control.hpp"
#include "userapi/automonous.hpp"

using namespace devices;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	chassis.calibrate();
	devices::chassis.setPose(0, 0, 90);
	
	PROSLogger::Manager::setLevel(PROSLogger::LogLevel::DEBUG);
	controls::configure();
	
	// ui::autom_selector::selected_color = ui::autom_selector::AutomColor::BLUE;
	ui::autom_selector::initialize();
	ui::driver::initialize();

	devices::opticalSensor.set_led_pwm(100);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	lv_screen_load(ui::autom_selector::autom_screen);
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	ui::autom_selector::run_automous();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	// lv_screen_load(ui::autom_selector::autom_screen);
	lv_screen_load(ui::driver::driver_screen);
	controls::button_handler.start();

	// Notifies Last 20 second park zone protect
	pros::Task notifier([](){
		pros::delay(75*1000);
		devices::controller.rumble("---");
		pros::delay(10*1000);
		devices::controller.rumble("-...-");
	});

	while (true) {
		if (keybindActions::drive::is_arcade() == true) {
			chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
		} else {
			chassis.tank(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
		}
		pros::delay(10);
	}
}