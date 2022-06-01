#pragma once

#include <pros/misc.hpp>

#include "Flywheel.hpp"
#include "Sensor.hpp"

class Robot {
private:
	pros::Controller m_controller{ pros::E_CONTROLLER_MASTER };
	Sensor m_sensor;
	Flywheel m_flywheel;

	// fulfills all controller interaction with the flywheel
	void update_flywheel();
public:
	Robot(Sensor sensor, Flywheel flywheel);

	// calls all update_x functions
	void update();
};
