#pragma once

#include <cstdint>
#include <pros/motors.hpp>

class Intake {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;
	bool m_on = false;
public:
	Intake(uint8_t const left_port, uint8_t const right_port);
	bool on() const;
	void set_on(bool);
	void toggle();
};
