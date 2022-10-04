#pragma once

#include <cstdint>
#include <pros/motors.hpp>

class Intake {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;
	bool m_on = false;
	bool m_reverse = false;

public:
	Intake(uint8_t const left_port, uint8_t const right_port);
	void set_on(bool const value);
	void toggle(bool const reverse);
	int32_t reverse_velocity(int32_t const velocity) const;

};
