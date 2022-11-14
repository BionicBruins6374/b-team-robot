#pragma once

#include "pros/adi.hpp"
#include "pros/motors.hpp"

class Flywheel {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;
	pros::ADIDigitalOut m_piston;

	bool m_on = false;
	bool m_reverse = true;
	
public:
	Flywheel(int8_t const left_port, int8_t const right_port, int8_t const piston_port);

	// spins the flywheel up to a neutral speed (constants::FLYWHEEL_ON_VELOCITY)
	void spin_up();

	// sets the flywheel motor to zero volage
	void disengage();

	// toggle active state of flywheel
	bool toggle_active(bool const reverse, bool const slower_voltage);

	// sets the flywheel velocity based on distance to the high goal
	void aim(uint8_t const velocity_option);

	// engages the piston to shoot a stored disc
	void shoot() const;
	
	// sets internal voltage, used to modify flywheel for range
	void set_voltage(int voltage) ;

	int32_t reverse_velocity(int32_t const velocity) const;


	

	
};
