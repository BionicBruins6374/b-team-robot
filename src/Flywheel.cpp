#include <cmath>
#include <cstdint>
#include <iostream>
#include "pros/rtos.hpp"

#include "Flywheel.hpp"

#include "constants.hpp"

Flywheel::Flywheel(int8_t const port, uint8_t const piston_port)
    : m_motor{ port, true }, m_piston{ piston_port } {}


void Flywheel::disengage() {
	m_motor.move_voltage(0);
	m_voltage = 0;
}

void Flywheel::aim(uint8_t const voltage_option) {
	if (constants::FLYWHEEL_VOLTAGES[voltage_option] == m_voltage) {
		disengage();
	} else {
		m_voltage = constants::FLYWHEEL_VOLTAGES[voltage_option];
		m_motor.move_voltage(m_voltage);
	}
	
}

void Flywheel::shoot() const {
	pros::Task{ [m_piston = this->m_piston] {
		m_piston.set_value(true);
		pros::Task::delay(constants::PISTON_DELAY_TIME);
		m_piston.set_value(false);
	} };
}