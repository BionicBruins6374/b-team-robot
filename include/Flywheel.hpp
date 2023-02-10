#pragma once

#include "pros/adi.hpp"
#include "pros/motors.hpp"

class Flywheel {
private:
	pros::Motor m_motor;
	pros::ADIDigitalOut m_piston;

public:
	int32_t m_voltage = 0;

	Flywheel(int8_t const port, uint8_t const piston_port);

	// sets the flywheel motor to zero volage
	void disengage();

	// sets the flywheel velocity based on distance to the high goal
	void aim(uint8_t const voltage_option);

	// engages the piston to shoot a stored disc
	void shoot() const;

};
