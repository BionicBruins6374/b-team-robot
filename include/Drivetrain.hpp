#pragma once

#include <cstdint>
#include <pros/motors.hpp>

enum class DrivetrainReferenceFrame {
	Normal,
	Reversed,
};

class Drivetrain {
private:
	pros::Motor m_left_back_motor;
	pros::Motor m_right_back_motor;
	pros::Motor m_left_front_motor;
	pros::Motor m_right_front_motor;
	DrivetrainReferenceFrame m_reference_frame = DrivetrainReferenceFrame::Normal;
public:
	Drivetrain(uint8_t left_back_motor_port, uint8_t right_back_motor_port, uint8_t left_front_motor_port, uint8_t right_front_motor_port);
	void update(int32_t forward_backward_axis, int32_t left_right_axis);
	void next_reference_frame();
};
