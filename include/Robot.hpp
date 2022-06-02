#pragma once

#include <pros/misc.hpp>

#include "Drivetrain.hpp"
#include "Flywheel.hpp"
#include "Sensor.hpp"

class Robot {
private:
	pros::Controller m_controller{ pros::E_CONTROLLER_MASTER };
	Drivetrain m_drivetrain;
	Sensor m_sensor;
	Flywheel m_flywheel;

	void update_drivetrain();
	// fulfills all controller interaction with the flywheel
	void update_flywheel();
public:
	Robot(Drivetrain drivetrain, Sensor sensor, Flywheel flywheel);

	// calls all update_x functions
	void update();
};
