#pragma once

#include <cstdint>
#include <pros/motors.hpp>

class Roller {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;

public:
	Roller(uint8_t const left_port, uint8_t const right_port);
	void switch_color() const;
	void fine_adjust(bool reverse) const;
};
