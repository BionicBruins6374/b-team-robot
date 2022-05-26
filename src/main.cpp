#include <cmath>
#include <iostream>
#include <optional>
#include <pros/rtos.hpp>
#include <pros/vision.hpp>
#include "main.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

namespace ports {
	constexpr int VISION = 1;
}

namespace measures {
	constexpr int GOAL_HEIGHT = 64; // 25 in to cm
	constexpr int SENSOR_HEIGHT = 44; // 17.5 in to cm
	constexpr float VERTICAL_FOV = 0.715585; // 41 deg to radians
	constexpr float LENS_ANGLE = 0.261799; // 15 deg to radians
}

constexpr int SIG_ERR = 255;

class Sensor {
private:
	pros::Vision m_sensor;
	pros::vision_signature_s_t m_sig;
public:
	Sensor(int port, pros::vision_signature_s_t sig): m_sensor(port), m_sig(sig) {
		m_sensor.set_signature(1, &m_sig);
	}

	// prints signature of largest object
	// used for calibrating sensor to a new object
	void print_sig() {
		if (m_sensor.get_object_count() > 0) {
			pros::vision_object_s_t rtn = m_sensor.get_by_size(0);
			pros::vision_signature_s_t sig = m_sensor.get_signature(rtn.signature);
			pros::Vision::print_signature(sig);
		} else {
			std::cout << "No objects found" << std::endl;
		}
	}

	// returns the object of the current signature
	std::optional<pros::vision_object_s_t> get_obj() {
		pros::vision_object_s_t rtn = m_sensor.get_by_sig(0, 1);
		if (rtn.signature == SIG_ERR) {
			return {};
		}
		return rtn;
	}

	// returns the distance to the current signature
	std::optional<float> get_dist() {
		std::optional<pros::vision_object_s_t> obj = get_obj();
		if (obj.has_value()) {
			int y_height = obj.value().y_middle_coord;
			float distance = measures::GOAL_HEIGHT - measures::SENSOR_HEIGHT / std::tan(measures::LENS_ANGLE - std::atan((1 - y_height / 200) * std::tan(measures::VERTICAL_FOV / 2)));
			return distance;
		}
		return {};
	}
};

void opcontrol() {
	// dummy signature for the high goal
	// will be replaced with real values later
	pros::vision_signature_s_t high_goal = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	Sensor vision = Sensor(ports::VISION, high_goal);

	while (true) {
		pros::Task::delay(1);
	}
}
