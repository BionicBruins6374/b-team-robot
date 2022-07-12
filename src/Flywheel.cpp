#include <cmath>
#include <cstdint>
#include <pros/rtos.hpp>

#include "Flywheel.hpp"

#include "constants.hpp"
#include "dimensions.hpp"

Flywheel::Flywheel(uint8_t const left_port, uint8_t const right_port, uint8_t const piston_port)
	: m_left_motor{ left_port }, m_right_motor{ right_port, true }, m_piston{ piston_port } {}

void Flywheel::spin_up() {
	m_left_motor.move_velocity(constants::FLYWHEEL_ON_VELOCITY);
	m_right_motor.move_velocity(constants::FLYWHEEL_ON_VELOCITY);
	m_on = true;
}

void Flywheel::disengage() {
	m_left_motor.move_voltage(0);
	m_right_motor.move_voltage(0);
	m_on = false;
}

void Flywheel::toggle_active() {
	if (m_on) {
		disengage();
	} else {
		spin_up();
	}
}

void Flywheel::aim(float const distance) const {
	float const secant = 1 / std::cos(dimensions::LAUNCH_ANGLE);
	float const numerator = (-1.0f * std::pow(distance, 2.0f) * constants::GRAVITY * std::pow(secant, 2.0f));
	float const denominator = (2 * (dimensions::GOAL_HEIGHT - (distance * secant * std::sin(dimensions::LAUNCH_ANGLE))) - dimensions::LAUNCH_HEIGHT);
	float const initial_velocity = std::sqrt(numerator / denominator);
	auto const flywheel_velocity = static_cast<int32_t>(initial_velocity * constants::FLYWHEEL_PROPORTION);
	m_left_motor.move_velocity(flywheel_velocity);
	m_right_motor.move_velocity(flywheel_velocity);
}

void Flywheel::shoot() const {
	pros::Task{ [m_piston = this->m_piston] {
		m_piston.set_value(true);
		pros::Task::delay(constants::PISTON_DELAY_TIME);
		m_piston.set_value(false);
	} };
}
