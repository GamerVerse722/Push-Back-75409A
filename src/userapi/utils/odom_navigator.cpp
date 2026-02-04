#include "userapi/utils/odom_navigator.hpp"
#include "okapi/api/units/QLength.hpp"
#include <format>

double OdomNavigator::normalize_x(okapi::QLength x_target) {
    double x = x_target.convert(okapi::inch);
    return chassis.odom_x_direction_get() ? -x : x;
}

double OdomNavigator::normalize_y(okapi::QLength y_target) {
    double y = y_target.convert(okapi::inch);
    return chassis.odom_y_direction_get() ? -y : y;
}

double OdomNavigator::normalize_theta(okapi::QAngle theta_target) {
    double theta = theta_target.convert(okapi::degree);
    return chassis.odom_theta_direction_get() ? theta : -theta;
}

void OdomNavigator::drive_to_x(okapi::QLength x_target, int speed) {
    double cos_theta = cos(chassis.odom_theta_get() * M_PI / 180.0);
    double dx = normalize_x(x_target) - chassis.odom_x_get();
    
    if (std::abs(cos_theta) < 0.05) return;
    
    double drive_distance = dx / cos_theta;
    
    // log.debug(std::format("X target: {:.2f}", normalize_x(x_target)));
    // log.debug(std::format("Distance to target: {:.2f}", dx));
    // log.debug(std::format("Cos Theta: {:.2f}", cos_theta));
    // log.debug(std::format("Drive distance: {:.2f}", std::abs(drive_distance)));
    chassis.pid_drive_set(std::abs(drive_distance), speed);
    chassis.pid_wait();
}

void OdomNavigator::drive_to_y(okapi::QLength y_target, int speed) {
    double sin_theta = sin(chassis.odom_theta_get() * M_PI / 180.0);
    double dy = normalize_y(y_target) - chassis.odom_y_get();

    if (std::abs(sin_theta) < 0.05) return;

    double drive_distance = dy / sin_theta;

    chassis.pid_drive_set(std::abs(drive_distance), speed);
    chassis.pid_wait();
}

void OdomNavigator::drive_to_x(okapi::QLength x_target, okapi::QAngle deg_target, int speed) {
    chassis.pid_turn_set(deg_target, speed);
    chassis.pid_wait();
    drive_to_x(x_target, speed);
}
void OdomNavigator::drive_to_y(okapi::QLength y_target, okapi::QAngle deg_target, int speed) {
    chassis.pid_turn_set(deg_target, speed);
    chassis.pid_wait();
    drive_to_x(y_target, speed);
}