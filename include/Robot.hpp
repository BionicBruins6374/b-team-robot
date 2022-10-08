#pragma once

#include <pros/misc.hpp>

#include "Drivetrain.hpp"
#include "Flywheel.hpp"
#include "Intake.hpp"
#include "Expansion.hpp"

class Robot {
private:
	pros::Controller m_controller{ pros::E_CONTROLLER_MASTER };
	Drivetrain m_drivetrain;
	Flywheel m_flywheel;
	Intake m_intake;
	Expansion m_expansion;

	void update_drivetrain();
	void update_flywheel();
	void update_intake();
	void update_expansion();
public:
	Robot(Drivetrain drivetrain, Flywheel flywheel, Intake intake, Expansion expansion);
	void update();
};
