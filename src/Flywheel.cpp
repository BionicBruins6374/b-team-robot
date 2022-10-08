#include <cmath>
#include <cstdint>
#include <pros/rtos.hpp>

#include "Flywheel.hpp"

#include "constants.hpp"

Flywheel::Flywheel(uint8_t const left_port, uint8_t const right_port, uint8_t const piston_port)
	: m_left_motor{ left_port, true }, m_right_motor{ right_port}, m_piston{ piston_port } {}

void Flywheel::spin_up() {
	m_left_motor.move_velocity(reverse_velocity(constants::FLYWHEEL_VELOCITIES[1]));
	m_right_motor.move_velocity(reverse_velocity(constants::FLYWHEEL_VELOCITIES[1]));
	m_on = true;
}

void Flywheel::disengage() {
	m_left_motor.move_voltage(0);
	m_right_motor.move_voltage(0);
	m_on = false;
}

void Flywheel::toggle_active(bool const reverse) {
	if (m_on) {
		disengage();
		m_reverse = false;
	} else {
		m_reverse = reverse;
		spin_up();
	}
}

void Flywheel::aim(uint8_t const velocity_option) {
	m_on = true;
	if (!m_reverse) { // transitioning from reverse flywheel velocity to forward flywheel velocity would damage motors
		int32_t velocity = constants::FLYWHEEL_VELOCITIES[velocity_option];
		m_left_motor.move_velocity(velocity);
		m_right_motor.move_velocity(velocity);
	}
}

void Flywheel::shoot() const {
	pros::Task{ [m_piston = this->m_piston] {
		m_piston.set_value(true);
		pros::Task::delay(constants::PISTON_DELAY_TIME);
		m_piston.set_value(false);
	} };
}

int32_t Flywheel::reverse_velocity(int32_t const velocity) const {
	if (m_reverse) {
		return -1 * velocity;
	}
	return velocity;
}
