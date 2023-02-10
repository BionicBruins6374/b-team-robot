#include "okapi/impl/chassis/controller/chassisControllerBuilder.hpp"
#include "okapi/impl/device/motor/motorGroup.hpp"
#include <cstdint>

using namespace okapi;

std::shared_ptr<OdomChassisController> build_odometry(MotorGroup left_motor, MotorGroup right_motor);