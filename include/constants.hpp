#pragma once

#include <cstdint>

// namespace for nonphysical and programmatical constants
namespace constants {
constexpr float CONTROLLER_ANALOG_MAX = 127.0f;  // maximum value for an analog axis on the controller
constexpr float DRIVE_DAMPENING = 0.5f;  // relative dampening for the drivetrain. 0.0..=1.0
constexpr float DRIVE_MAX_VELOCITY = 200.0f;  // based on type of gear cartridge in drive motors
constexpr float FLYWHEEL_PROPORTION = 1.0;  // (needed disc velocity) * (FLYWHEEL_PROPORTION) = (needed motor velocity)
constexpr float GRAVITY = 9.79909f;  // local gravity in m/s/s
constexpr float SENSOR_VERTICAL_FOV = 0.715585f;  // 41 deg to radians
constexpr int32_t FLYWHEEL_ON_VELOCITY = 100;  // the default flywheel velocity when not aimed
constexpr uint32_t PISTON_DELAY_TIME = 500;  // number of milliseconds to wait between extension and retraction of piston indexer
constexpr uint32_t SIG_ERR = 255;  // the ID value of a nonexistant/errored vision signature
}  // namespace constants
