#include <pros/vision.hpp>
#include <pros/rtos.hpp>
#include "main.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

#define VISION_PORT 1

void opcontrol() {
	pros::Vision vision_sensor(VISION_PORT);

	while (true) {
		pros::Task::delay(1); 
	}
}
