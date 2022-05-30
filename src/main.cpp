#include <cmath>
#include <iostream>
#include <pros/rtos.hpp>
#include <cstdint>
#include <pros/motors.hpp>
#include <pros/misc.hpp>

#include "main.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

Sensor::Sensor(uint8_t const port, pros::vision_signature_s_t const sig) : m_sensor{port}, m_sig(sig) {
	m_sensor.set_signature(1, &m_sig);
}

void Sensor::print_sig() const {
	if (m_sensor.get_object_count() > 0) {
		auto const rtn = m_sensor.get_by_size(0);
		auto const sig = m_sensor.get_signature(rtn.signature);
		pros::Vision::print_signature(sig);
	} else {
		std::cout << "No objects found" << std::endl;
	}
}

std::optional<pros::vision_object_s_t> Sensor::get_obj() const {
	auto const rtn = m_sensor.get_by_sig(0, 1);
	if (rtn.signature == constants::SIG_ERR) {
		return {};
	} else {
		return rtn;
	}
}

std::optional<float> Sensor::get_dist() const {
	auto const obj = get_obj();
	if (obj.has_value()) {
		int const y_height = obj.value().y_middle_coord;
		return dimensions::GOAL_HEIGHT - dimensions::SENSOR_HEIGHT / std::tan(dimensions::LENS_ANGLE - std::atan((1 - y_height / 200) * std::tan(constants::SENSOR_VERTICAL_FOV / 2)));
	}
	return {};
}

Flywheel::Flywheel(uint8_t const left_port, uint8_t const right_port, uint8_t const piston_port) : m_left_motor{left_port}, m_right_motor{right_port}, m_piston{piston_port} {}

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

void Flywheel::aim(float const distance) const {
	float const secant = 1/std::cos(dimensions::LAUNCH_ANGLE);
	int16_t velocity = std::sqrt((-1 * std::pow(distance, 2) * constants::GRAVITY * std::pow(secant, 2))/(2 * (dimensions::GOAL_HEIGHT - (distance * secant * std::sin(dimensions::LAUNCH_ANGLE))) - dimensions::LAUNCH_HEIGHT)) * constants::FLYWHEEL_PROPORTION;
	m_left_motor.move_velocity(velocity);
	m_right_motor.move_velocity(velocity);
}

void Flywheel::shoot() const {
	m_piston.set_value(true);
	pros::Task::delay(constants::PISTON_DELAY_TIME);
	m_piston.set_value(false);
}

Robot::Robot(Sensor sensor, Flywheel flywheel) : m_sensor(sensor), m_flywheel(flywheel) {}

void Robot::update_flywheel() {
	if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
		auto const dist = m_sensor.get_dist();
		if (dist.has_value()) {
			m_flywheel.aim(dist.value());
		} else {
			m_controller.rumble(".");
		}
	} else if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
		m_flywheel.shoot();
	} else if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		if (!m_flywheel.m_on) {
			m_flywheel.spin_up();
		} else {
			m_flywheel.disengage();
		}
	}
}

void Robot::update() {
	update_flywheel();
}

void opcontrol() {
	// dummy signature for the high goal
	// will be replaced with real values later
	auto const high_goal = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	
	Sensor const vision{ ports::VISION, high_goal };
	Flywheel const flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Robot robot{ vision, flywheel };

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}
