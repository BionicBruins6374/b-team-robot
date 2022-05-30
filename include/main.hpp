#pragma once
#include <cstdint>
#include <optional>
#include <pros/vision.hpp>
#include <pros/motors.hpp>
#include <pros/adi.hpp>

extern "C" {
void initialize();
void disabled();
void competition_initialize();
void autonomous();
void opcontrol();
}

// namespace for port connections to V5 devices
namespace ports {
constexpr uint8_t VISION = 1;
constexpr uint8_t FLYWHEEL_LEFT = 2;
constexpr uint8_t FLYWHEEL_RIGHT = 3;
constexpr uint8_t PISTON_INDEXER = 'A';
}

// namespace for phsyical robot and field dimensions
namespace dimensions {
constexpr float GOAL_HEIGHT = 63.5;  // 25 in to cm
constexpr float SENSOR_HEIGHT = 44.45;  // 17.5 in to cm
constexpr float LENS_ANGLE = 0.261799;  // 15 deg to radians
constexpr float LAUNCH_ANGLE = 0.785398; // 45 deg to radians
constexpr float LAUNCH_HEIGHT = 38.1; // 15 in to cm
}

// namespace for nonphysical and programmatical constants
namespace constants {
constexpr float FLYWHEEL_PROPORTION = 1.0; // (needed disc velocity) * (FLYWHEEL_PROPORTION) = (needed motor velocity)
constexpr int16_t FLYWHEEL_ON_VELOCITY = 100; // the default flywheel velocity when not aimed
constexpr float GRAVITY = 9.79909; // local gravity in m/s/s
constexpr float SENSOR_VERTICAL_FOV = 0.715585;  // 41 deg to radians
constexpr uint32_t PISTON_DELAY_TIME = 500; // number of milliseconds to wait between extension and retraction of piston indexer
constexpr uint32_t SIG_ERR = 255; // the ID value of a nonexistant/errored vision signature
}

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

class Flywheel {
private:
	pros::Motor m_left_motor;
	pros::Motor m_right_motor;
	pros::ADIDigitalOut m_piston;
public:
	Flywheel(uint8_t const left_port, uint8_t const right_port, uint8_t const piston_port);

	// spins the flywheel up to a neutral speed (constants::FLYWHEEL_ON_VELOCITY)
	void spin_up() const;

	// sets the flywheel motor to zero volage
	void disengage() const;

	// sets the flywheel velocity based on distance to the high goal
	void aim(float const distance) const;

	// engages the piston to shoot a stored disc
	void shoot() const;
};
