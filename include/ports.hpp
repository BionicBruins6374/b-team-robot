#pragma once

#include <cstdint>

// namespace for port connections to V5 devices
namespace ports {
constexpr int8_t FLYWHEEL_LEFT = 15;
constexpr int8_t FLYWHEEL_RIGHT = 12;

constexpr int8_t PISTON_INDEXER = 'A';
constexpr int8_t EXPANSION_PISTON = 'B';


constexpr int8_t LEFT_BACK_MOTOR = 2;
constexpr int8_t RIGHT_BACK_MOTOR = 1;
constexpr int8_t LEFT_FRONT_MOTOR = 10;
constexpr int8_t RIGHT_FRONT_MOTOR = 9;
constexpr int8_t INTAKE_LEFT = 13;
constexpr int8_t INTAKE_RIGHT = 5;
}  // namespace ports
