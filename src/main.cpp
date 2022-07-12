#include <pros/rtos.hpp>
#include <pros/vision.hpp>

#include "Flywheel.hpp"
#include "Robot.hpp"
#include "Sensor.hpp"
#include "Intake.hpp"
#include "Roller.hpp"

#include "main.hpp"
#include "ports.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
	// dummy signature for the high goal
	// will be replaced with real values later
	auto const high_goal = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4f, 0);

	Drivetrain const drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Sensor const vision{ ports::VISION, high_goal };
	Flywheel const flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Intake const intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Roller const roller{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };


	Robot robot{ drivetrain, vision, flywheel, intake, roller};

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}
