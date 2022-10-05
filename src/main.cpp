#include <pros/rtos.hpp>
#include <pros/vision.hpp>

#include "Flywheel.hpp"
#include "Robot.hpp"
#include "Intake.hpp"
#include "Roller.hpp"

#include "main.hpp"
#include "ports.hpp"

#include "odometry.hpp"

#include <okapi/impl/device/motor/motor.hpp>
#include <okapi/impl/device/motor/motorGroup.hpp>

void initialize() {}
void disabled() {}
void competition_initialize() {}

void opcontrol() {
	Drivetrain const drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Flywheel const flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Intake const intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Roller const roller{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };


	Robot robot{ drivetrain, flywheel, intake, roller};

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}

void autonomous() {
	auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(2), okapi::Motor(10)}), 
								   okapi::MotorGroup({okapi::Motor(1), okapi::Motor(9)}));

	odometry->driveToPoint({1_ft, 1_ft});
}
