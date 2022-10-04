#include <pros/rtos.hpp>
#include <pros/vision.hpp>

#include "Flywheel.hpp"
#include "Robot.hpp"
#include "Intake.hpp"
#include "Roller.hpp"

#include "main.hpp"
#include "ports.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

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
