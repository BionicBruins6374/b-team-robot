#pragma once

#include <cstdint>

// namespace for nonphysical and programmatical constants
namespace constants {
constexpr float CONTROLLER_ANALOG_MAX = 127.0f;  // maximum value for an analog axis on the controller
constexpr float DRIVE_DAMPENING = 0.75f;  // relative dampening for the drivetrain. 0.0..=1.0
constexpr float DRIVE_MAX_VELOCITY = 200.0f;  // fastest drivetrain velocity
constexpr int32_t FLYWHEEL_VELOCITIES[3] = {100, 200};  // constant flywheel velocities to choose from
constexpr uint32_t PISTON_DELAY_TIME = 250;  // number of milliseconds to wait between extension and retraction of piston indexer
constexpr int32_t INTAKE_VELOCITY = 200; // intake motor velocity
}  // namespace constants
