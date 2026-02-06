#pragma once

#include "EZ-Template/drive/drive.hpp"

#include "gamers-forge/proslogger.hpp"

class OdomNavigator {
    public:
        explicit OdomNavigator(ez::Drive& drive): chassis(drive) {}

        void record();
        void reset_x();
        void reset_y();

        void drive_to_x(okapi::QLength x_target, int speed);
        void drive_to_y(okapi::QLength y_target, int speed);
        
        void drive_to_x(okapi::QLength x_target, okapi::QAngle deg_target, int speed);
        void drive_to_y(okapi::QLength y_target, okapi::QAngle deg_target, int speed);

    private:
        ez::Drive& chassis;

        double x_start = 0;
        double y_start = 0;
        double d_start = 0;

        PROSLogger::Logger log{"OdomNavigator"};
        
        double normalize_x(okapi::QLength x_target);
        double normalize_y(okapi::QLength y_target);
        double normalize_theta(okapi::QAngle theta_target);
};