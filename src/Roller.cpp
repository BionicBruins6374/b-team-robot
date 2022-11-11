#include "Roller.hpp"
#include "constants.hpp"

Roller::Roller(int8_t const left_port, int8_t const right_port) : m_left_motor{ left_port }, m_right_motor { right_port, true } {
	m_left_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	m_right_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
}

void Roller::switch_color() const {
	// moves motor 180 degrees forward at a fast pace (switches roller color)
	m_left_motor.move_relative(540, 100);
	m_right_motor.move_relative(540, 100);
}

void Roller::fine_adjust(RollerCode code) const {
	switch (code) {
		case FORWARD:
			m_left_motor.move_velocity(30);
			m_right_motor.move_velocity(30);
			break;
		case BACKWARD:
			m_left_motor.move_velocity(-30);
			m_right_motor.move_velocity(-30);
			break;
		case STOP:
			m_left_motor.move_velocity(0);
			m_right_motor.move_velocity(0);
			break;
	};
}

void Roller::spin_wheel(bool on) const{
	if (on) {
		m_left_motor.move_velocity(100);
		m_right_motor.move_velocity(100);
	}

	else {
		m_left_motor.move_velocity(0);
		m_right_motor.move_velocity(0);
	}
}