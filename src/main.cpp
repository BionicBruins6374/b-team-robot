#include <cmath>
#include <iostream>
#include <pros/rtos.hpp>

#include "main.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

Sensor::Sensor(int const port, pros::vision_signature_s_t const sig) : m_sensor(port), m_sig(sig) {
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
	if (rtn.signature == SIG_ERR) {
		return {};
	} else {
		return rtn;
	}
}

std::optional<float> Sensor::get_dist() const {
	auto const obj = get_obj();
	if (obj.has_value()) {
		int const y_height = obj.value().y_middle_coord;
		return measures::GOAL_HEIGHT - measures::SENSOR_HEIGHT / std::tan(measures::LENS_ANGLE - std::atan((1 - y_height / 200) * std::tan(measures::VERTICAL_FOV / 2)));
	}
	return {};
}

void opcontrol() {
	// dummy signature for the high goal
	// will be replaced with real values later
	auto const high_goal = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	Sensor const vision{ ports::VISION, high_goal };

	while (true) {
		pros::Task::delay(1);
	}
}
