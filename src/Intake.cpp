#include "Intake.hpp"

#include "constants.hpp"

Intake::Intake(uint8_t const left_port, uint8_t const right_port) : m_left_motor{ left_port }, m_right_motor{ right_port, true } {}

bool Intake::on() const {
	return m_on;
}

void Intake::set_on(bool value) {
	m_on = value;
	auto velocity = 0;
	if (m_on) {
		velocity = constants::INTAKE_VELOCITY;
	}
	m_left_motor.move_velocity(velocity);
	m_right_motor.move_velocity(velocity);
}

void Intake::toggle() {
	set_on(!on());
}
