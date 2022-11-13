#include "Roller.hpp"
#include "constants.hpp"

Roller::Roller(int8_t const left_port, int8_t const right_port, int8_t const sole_port) : m_left_motor{ left_port }, m_right_motor { right_port }, 
m_sole_motor {sole_port, true} {
	m_left_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	m_right_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	m_sole_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
}

void Roller::switch_color() const {
	// moves motor 180 degrees forward at a fast pace (switches roller color)
	m_left_motor.move_relative(270, 100);
	m_right_motor.move_relative(270, 100);
	m_sole_motor.move_relative(270, 100);
}

void Roller::fine_adjust(bool reverse) const {
	if (reverse) {
		// move at a slow pace backwards
		m_left_motor.move_relative(-30, 80);
		m_right_motor.move_relative(-30, 80);
		m_sole_motor.move_relative(-30, 80);
	} else {
		// move at a slow pace forward
		m_left_motor.move_relative(30, 80);
		m_right_motor.move_relative(30, 80);
		m_sole_motor.move_relative(30, 80);
	}
	pros::Task{ [] {pros::Task::delay(350);} };

}
void Roller::spin() {
	if (!m_on) {
		m_sole_motor.move_velocity(constants::INTAKE_VELOCITY);
		m_on = true;
	}
	else {
		m_sole_motor.move_velocity(0);
		m_on = false;
	}
}