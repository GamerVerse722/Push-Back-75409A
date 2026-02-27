#include "main.h"

#include "EZ-Template/util.hpp"
#include "gamers-forge/proslogger.hpp"

#include "liblvgl/display/lv_display.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

#include "userapi/controls/drive.hpp"
#include "userapi/configuration.hpp"
#include "userapi/controls/intake.hpp"
#include "userapi/ui/autom/mode_selector.hpp"
#include "userapi/ui/op_control.hpp"


using namespace devices;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	using namespace ui::autom;
	PROSLogger::Manager::setLevel(PROSLogger::LogLevel::DEBUG);

	pros::delay(500);

	ui::driver::initialize();
	
	configuration::drive::initialize();
	configuration::controls::configure();
	configuration::autonomous::configure();
	
	AutoManager::select_autom(AutomMode::AWP, AutomSideColor::RED_RIGHT);
	// AutoManager::select_autom(AutomMode::ELIMINATIONS, AutomSideColor::RED_RIGHT);
	// AutoManager::select_autom(AutomMode::ELIMINATIONS, AutomSideColor::RED_LEFT);

	// lv_screen_load(ui::autom::mode_selector::mode_screen);
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
	lv_screen_load(ui::autom::mode_selector::mode_screen);
	// lv_screen_load(ui::driver::driver_screen);
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
	using namespace ui::autom;
	// lv_screen_load(ui::driver::driver_screen);
	select_autom(AutomMode::QUALIFICATIONS, AutomSideColor::RED_RIGHT);
	AutoManager::run_autom();
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
	lv_screen_load(ui::autom::mode_selector::mode_screen);
	// lv_screen_load(ui::driver::driver_screen);

	keybindActions::intake::reset_low_score_speed();
	configuration::controls::button_handler.start();

	chassis.drive_brake_set(MOTOR_BRAKE_COAST);

	// Notifies Last 20 second park zone protect
	pros::Task notifier([](){
		pros::delay(75*1000);
		master.rumble("---");
		pros::delay(10*1000);
		master.rumble("-...-");
	});

	while (true) {
		// chassis.opcontrol_tank();
		if (keybindActions::drive::is_arcade() == true) {
			chassis.opcontrol_arcade_standard(ez::SPLIT);
		} else {
			chassis.opcontrol_tank();
		}
		pros::delay(ez::util::DELAY_TIME);
	}
}