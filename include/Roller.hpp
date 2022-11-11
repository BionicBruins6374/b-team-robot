#pragma once

#include <cstdint>
#include "pros/motors.hpp"

class Roller {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;

public:
	enum RollerCode {
		FORWARD = 0,
		BACKWARD = 1,
		STOP = 2
	};

	Roller(int8_t const left_port, int8_t const right_port);
	void switch_color() const;
	void fine_adjust(RollerCode code) const;
	void spin_wheel(bool on) const;	
};