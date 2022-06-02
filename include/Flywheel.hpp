#pragma once

#include <pros/adi.hpp>
#include <pros/motors.hpp>

class Flywheel {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;
	pros::ADIDigitalOut m_piston;

	bool m_on = false;
public:
	Flywheel(uint8_t const left_port, uint8_t const right_port, uint8_t const piston_port);

	// spins the flywheel up to a neutral speed (constants::FLYWHEEL_ON_VELOCITY)
	void spin_up();

	// sets the flywheel motor to zero volage
	void disengage();

	// toggle active state of flywheel
	void toggle_active();

	// sets the flywheel velocity based on distance to the high goal
	void aim(float const distance) const;

	// engages the piston to shoot a stored disc
	void shoot() const;
};
