#pragma once
#include <cstdint>
#include <optional>
#include <pros/vision.hpp>

extern "C" {
void initialize();
void disabled();
void competition_initialize();
void autonomous();
void opcontrol();
}

namespace ports {
constexpr int VISION = 1;
}

namespace measures {
constexpr int GOAL_HEIGHT = 64;  // 25 in to cm
constexpr int SENSOR_HEIGHT = 44;  // 17.5 in to cm
constexpr float VERTICAL_FOV = 0.715585;  // 41 deg to radians
constexpr float LENS_ANGLE = 0.261799;  // 15 deg to radians
}  // namespace measures

constexpr uint32_t SIG_ERR = 255;

class Sensor {
private:
	pros::Vision m_sensor;
	pros::vision_signature_s_t m_sig;
public:
	Sensor(int const port, pros::vision_signature_s_t const sig);

	// prints signature of largest object
	// used for calibrating sensor to a new object
	void print_sig() const;

	// returns the object of the current signature
	std::optional<pros::vision_object_s_t> get_obj() const;

	// returns the distance to the current signature
	std::optional<float> get_dist() const;
};
