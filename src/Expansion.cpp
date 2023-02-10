#include <cmath>
#include <cstdint>
#include "pros/adi.hpp"

#include "Expansion.hpp"

Expansion::Expansion(uint8_t const piston_port_left, uint8_t const piston_port_right): 
	m_piston_left{ piston_port_left }, m_piston_right{ piston_port_right } {}

void Expansion::trigger() {
	m_count += 1;
	if (m_count == 4) {
		m_piston_left.set_value(true);
		m_piston_right.set_value(true);
	}
}