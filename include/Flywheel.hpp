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
	void spin_up();
	void disengage();
	void toggle_active();
	// sets the flywheel velocity based on distance to the high goal
	void aim(float const distance) const;
	// engages the piston to shoot a stored disc
	void shoot() const;
};
