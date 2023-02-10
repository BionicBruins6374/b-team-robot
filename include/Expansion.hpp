#pragma once

#include "pros/adi.hpp"


class Expansion {
private:
	pros::ADIDigitalOut m_piston_left;
	pros::ADIDigitalOut m_piston_right;

	uint8_t m_count = 0;
public:
	Expansion(uint8_t const piston_left, uint8_t const piston_right);

	void trigger();
};
