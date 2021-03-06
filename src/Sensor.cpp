#include <cmath>
#include <iostream>

#include "Sensor.hpp"

#include "constants.hpp"
#include "dimensions.hpp"

Sensor::Sensor(uint8_t const port, pros::vision_signature_s_t const sig) : m_sensor{ port }, m_sig(sig) {
	m_sensor.set_signature(1, &m_sig);
}

std::optional<pros::vision_object_s_t> Sensor::get_obj() const {
	auto const rtn = m_sensor.get_by_sig(0, 1);
	if (rtn.signature == constants::SIG_ERR) {
		return {};
	} else {
		return rtn;
	}
}

std::optional<float> Sensor::get_distance() const {
	auto const obj = get_obj();
	if (obj.has_value()) {
		float const y_height = obj.value().y_middle_coord;
		float const numerator = dimensions::GOAL_HEIGHT - dimensions::SENSOR_HEIGHT;
		float const denominator = std::tan(dimensions::LENS_ANGLE - std::atan((1 - y_height / 200) * std::tan(constants::SENSOR_VERTICAL_FOV / 2)));
		return numerator / denominator;
	}
	return {};
}
