#include <okapi/impl/chassis/controller/chassisControllerBuilder.hpp>
#include <cstdint>

using namespace okapi;

std::shared_ptr<OdomChassisController> build_odometry(uint8_t left_motor, uint8_t right_motor) {
	return okapi::ChassisControllerBuilder()
		.withMotors(left_motor, right_motor)
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR})
		.withOdometry() 
		.buildOdometry(); 
}