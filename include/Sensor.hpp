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

	// prints signature of largest object
	// used for calibrating sensor to a new object
	void print_sig() const;

	// returns the object of the current signature
	std::optional<pros::vision_object_s_t> get_obj() const;

	// returns the distance to the current signature
	std::optional<float> get_dist() const;
};
