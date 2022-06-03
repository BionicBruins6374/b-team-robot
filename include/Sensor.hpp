#pragma once

#include <cstdint>
#include <optional>
#include <pros/vision.hpp>

class Sensor {
private:
	pros::Vision m_sensor;
	pros::vision_signature_s_t m_sig;
public:
	Sensor(uint8_t const port, pros::vision_signature_s_t const sig);
	void print_largest() const;
	std::optional<pros::vision_object_s_t> get_object() const;
	std::optional<float> get_distance() const;
};
