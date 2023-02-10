#include "okapi/impl/chassis/controller/chassisControllerBuilder.hpp"
#include "okapi/impl/device/motor/motorGroup.hpp"
#include <cstdint>

using namespace okapi;

std::shared_ptr<OdomChassisController> build_odometry(MotorGroup left_motor, MotorGroup right_motor) {
	return okapi::ChassisControllerBuilder()
		.withMotors(left_motor, right_motor)
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
		.withOdometry() 
		.buildOdometry(); 
}