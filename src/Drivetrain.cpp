#include <cmath>

#include "Drivetrain.hpp"

#include "constants.hpp"

Drivetrain::Drivetrain(uint8_t const left_back_motor_port, uint8_t const right_back_motor_port, uint8_t const left_front_motor_port, uint8_t const right_front_motor_port)
	: m_left_back_motor{ left_back_motor_port }
	, m_right_back_motor{ right_back_motor_port }
	, m_left_front_motor{ left_front_motor_port }
	, m_right_front_motor{ right_front_motor_port } {}

static float scale(float const raw) {
	return std::pow(raw / 127.0f, 3.0f) * 100.0f * constants::DRIVE_DAMPENING;
}

void Drivetrain::update(int32_t forward_backward_axis_int, int32_t left_right_axis_int) {
	switch (m_reference_frame) {
		case DrivetrainReferenceFrame::Normal:
			break;
		case DrivetrainReferenceFrame::Reversed:
			forward_backward_axis_int *= -1;
			left_right_axis_int *= -1;
			break;
	}

	auto forward_backward_axis = static_cast<float>(forward_backward_axis_int);
	auto left_right_axis = static_cast<float>(left_right_axis_int);

	forward_backward_axis = scale(forward_backward_axis);
	left_right_axis = scale(left_right_axis);

	auto const left_velocity = static_cast<int32_t>(forward_backward_axis + left_right_axis);
	auto const right_velocity = static_cast<int32_t>(forward_backward_axis - left_right_axis);

	m_left_back_motor.move_velocity(left_velocity);
	m_left_front_motor.move_velocity(left_velocity);

	m_right_back_motor.move_velocity(right_velocity);
	m_right_front_motor.move_velocity(right_velocity);
}

void Drivetrain::next_reference_frame() {
	switch (m_reference_frame) {
		case DrivetrainReferenceFrame::Normal:
			m_reference_frame = DrivetrainReferenceFrame::Reversed;
			break;
		case DrivetrainReferenceFrame::Reversed:
			m_reference_frame = DrivetrainReferenceFrame::Normal;
			break;
	}
}
