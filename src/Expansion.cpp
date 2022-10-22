#include <cmath>
#include <cstdint>
#include "pros/adi.hpp"

#include "Expansion.hpp"

Expansion::Expansion(int8_t const piston_port): m_piston{ piston_port } {}

void Expansion::trigger() {
	m_count += 1;
	if (m_count == 4) {
		m_piston.set_value(true);
	}
}