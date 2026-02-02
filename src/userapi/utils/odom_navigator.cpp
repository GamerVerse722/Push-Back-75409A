#include "userapi/utils/odom_navigator.hpp"

double OdomNavigator::normalize_x(okapi::QLength x_target) {
    double x = x_target.convert(okapi::inch);
    return chassis.odom_x_direction_get() ? -x : x;
}

double OdomNavigator::normalize_y(okapi::QLength y_target) {
    double y = y_target.convert(okapi::inch);
    return chassis.odom_y_direction_get() ? -y : y;
}

double OdomNavigator::normalize_theta(okapi::QAngle theta_target) {
    double theta = theta_target.convert(okapi::radian);
    return chassis.odom_theta_direction_get() ? theta : -theta;
}

void OdomNavigator::drive_to_x(okapi::QLength x_target, int speed) {
    double cos_theta = cos(chassis.odom_theta_get() * M_PI / 180.0);
    double dx = normalize_x(x_target) - chassis.odom_x_get();

    if (std::abs(cos_theta) < 0.05) return;

    double drive_distance = dx / cos_theta;

    chassis.pid_drive_set(drive_distance, speed);
    chassis.pid_wait();
}

void OdomNavigator::drive_to_y(okapi::QLength y_target, int speed) {
    double sin_theta = sin(chassis.odom_theta_get() * M_PI / 180.0);
    double dx = normalize_x(y_target) - chassis.odom_x_get();

    if (std::abs(sin_theta) < 0.05) return;

    double drive_distance = dx / sin_theta;

    chassis.pid_drive_set(drive_distance, speed);
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