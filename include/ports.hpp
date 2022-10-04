#pragma once

#include <cstdint>

// namespace for port connections to V5 devices
namespace ports {
constexpr uint8_t FLYWHEEL_LEFT = 15;
constexpr uint8_t FLYWHEEL_RIGHT = 12;

constexpr uint8_t PISTON_INDEXER = 'A';

constexpr uint8_t LEFT_BACK_MOTOR = 2;
constexpr uint8_t RIGHT_BACK_MOTOR = 1;
constexpr uint8_t LEFT_FRONT_MOTOR = 10;
constexpr uint8_t RIGHT_FRONT_MOTOR = 9;
constexpr uint8_t INTAKE_LEFT = 13;
constexpr uint8_t INTAKE_RIGHT = 5;
}  // namespace ports
