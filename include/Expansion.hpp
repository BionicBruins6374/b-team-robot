#pragma once

#include "pros/adi.hpp"


class Expansion {
private:
	pros::ADIDigitalOut m_piston;
	uint8_t m_count = 0;
public:
	Expansion(int8_t const piston_port);

	void trigger();
};
