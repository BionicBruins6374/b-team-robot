#include "Roller.hpp"
#include "constants.hpp"

Roller::Roller(uint8_t const left_port, uint8_t const right_port) : m_left_motor{ left_port }, m_right_motor { right_port, true } {
	m_left_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	m_right_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
}

void Roller::switch_color() const {
	// moves motor 180 degrees forward at a fast pace (switches roller color)
	m_left_motor.move_relative(180, 100);
	m_right_motor.move_relative(180, 100);
}

void Roller::fine_adjust(bool const reverse) const {
	if (reverse) {
		// move at a medium pace backwards
		m_left_motor.move_relative(-10, 35);
		m_right_motor.move_relative(-10, 35);
	} else {
		// move at a medium pace forward
		m_left_motor.move_relative(10, 35);
		m_right_motor.move_relative(10, 35);
	}
}
