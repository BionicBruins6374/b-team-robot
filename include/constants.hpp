#pragma once

#include <cstdint>

// namespace for nonphysical and programmatical constants
namespace constants {
constexpr float FLYWHEEL_PROPORTION = 1.0;  // (needed disc velocity) * (FLYWHEEL_PROPORTION) = (needed motor velocity)
constexpr int16_t FLYWHEEL_ON_VELOCITY = 100;  // the default flywheel velocity when not aimed
constexpr float GRAVITY = 9.79909;  // local gravity in m/s/s
constexpr float SENSOR_VERTICAL_FOV = 0.715585;  // 41 deg to radians
constexpr uint32_t PISTON_DELAY_TIME = 500;  // number of milliseconds to wait between extension and retraction of piston indexer
constexpr uint32_t SIG_ERR = 255;  // the ID value of a nonexistant/errored vision signature
}  // namespace constants
