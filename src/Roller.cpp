#include "Roller.hpp"
#include "constants.hpp"

Roller::Roller(int8_t const port) : m_motor{ port } {
	m_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
}

void Roller::switch_color() const {
	// moves motor 180 degrees forward at a fast pace (switches roller color)
	m_motor.move_relative(180, 100);
}

void Roller::fine_adjust(RollerCode code) {
	switch (code) {
		case FORWARD:
			m_motor.move_velocity(80);
			m_on = true;
			break;
		case BACKWARD:
			m_motor.move_velocity(-80);
			m_on = true;
			break;
		case STOP:
			if (m_on == true) {
				m_motor.move_velocity(0);
				m_on = false;
			}
			break;
	};
}

void Roller::spin_wheel(bool on) const{
	if (on) {
		m_motor.move_velocity(100);
	}

	else {
		m_motor.move_velocity(0);
	}
}