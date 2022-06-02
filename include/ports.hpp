#pragma once

#include <cstdint>

// namespace for port connections to V5 devices
namespace ports {
constexpr uint8_t VISION = 1;
constexpr uint8_t FLYWHEEL_LEFT = 2;
constexpr uint8_t FLYWHEEL_RIGHT = 3;
constexpr uint8_t PISTON_INDEXER = 'A';
constexpr uint8_t LEFT_BACK_MOTOR = 4;
constexpr uint8_t RIGHT_BACK_MOTOR = 4;
constexpr uint8_t LEFT_FRONT_MOTOR = 4;
constexpr uint8_t RIGHT_FRONT_MOTOR = 4;
}  // namespace ports
