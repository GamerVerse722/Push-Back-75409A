#include "main.h"

// #include "pros/optical.hpp"
// #include "gamers-forge/bmapper.hpp"
// #include "liblvgl/llemu.hpp"
#include "gamers-forge/proslogger.hpp"
#include "pros/rtos.hpp"
#include "pros/misc.h"

// #include "liblvgl/lv_api_map_v8.h"
// #include "liblvgl/misc/lv_color.h"
// #include "liblvgl/core/lv_obj.h"

// #include "userapi/handler/optical_normalize.hpp"
// #include "userapi/handler/image_handler.hpp"
#include "userapi/configuration.hpp"

#include "userapi/controls/drive.hpp"

// #include <sstream>

using namespace devices;
// LV_IMAGE_DECLARE(AmongUsScaled);
// LV_IMAGE_DECLARE(PioneerContainerService);

// images::ImageHandler loaded_images(5000);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	
	PROSLogger::Manager::setLevel(PROSLogger::LogLevel::DEBUG);

	// loaded_images.register_image(AmongUsScaled, 100);
	// loaded_images.register_image(PioneerContainerService, 10000);
	// loaded_images.start();


	// pros::Task screen([&]{
	// 	lv_obj_t* container = lv_obj_create(lv_screen_active());
	// 	lv_obj_set_size(container, 480, 240);
	// 	lv_obj_center(container);

	// 	lv_obj_set_scroll_dir(container, LV_DIR_VER);
	// 	lv_obj_set_scroll_snap_y(container, LV_SCROLL_SNAP_CENTER);
	// 	lv_obj_set_style_max_height(container, 1000, 0);
	// 	lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
		
	// 	lv_obj_t* label = lv_label_create(container);
	// 	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

	// 	using namespace devices;
	// 	opticalSensor.set_led_pwm(100);

	// 	while (true) {
	// 		std::ostringstream oss;
	// 		pros::c::optical_rgb_s_t rgb = opticalSensor.get_rgb();
	// 		pros::c::optical_rgb_s_t normal_rgb = optical_normalize(opticalSensor.get_rgb());

	// 		oss << std::fixed << std::setprecision(2);
	// 		oss << "X: " << chassis.getPose().x << ", Y: " << chassis.getPose().y << ", Theta: " << chassis.getPose().theta << "\n";
	// 		oss << "Red: " << rgb.red << ", Green: " << rgb.green << ", Blue: " << rgb.blue << ", Brightness: " << rgb.brightness << "\n";
	// 		oss << "Red: " << normal_rgb.red << ", Green: " << normal_rgb.green << ", Blue: " << normal_rgb.blue << ", Brightness: " << normal_rgb.brightness << "\n";

	// 		lv_label_set_text(label, oss.str().c_str());

	// 		lv_obj_move_foreground(container);

	// 		pros::delay(10);
	// 	}
	// });
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
void competition_initialize() {}

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
	// using namespace devices;
	// chassis.setPose(0, 0, 0);
	// chassis.turnToHeading(90, 100000);
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
	controls::configure();

	while (true) {
		if (keybindActions::drive::is_arcade() == true) {
			chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
		} else {
			chassis.tank(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
		}
		pros::delay(10);
	}
	
}