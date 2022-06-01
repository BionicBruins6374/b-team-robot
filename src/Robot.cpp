#include "Robot.hpp"

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
