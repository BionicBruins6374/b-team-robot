#include <pros/rtos.hpp>
#include <pros/vision.hpp>

#include "Flywheel.hpp"
#include "Robot.hpp"
#include "Sensor.hpp"

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

	Sensor const vision{ ports::VISION, high_goal };
	Flywheel const flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Robot robot{ vision, flywheel };

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}
