#include "Drivetrain.hpp"

Drivetrain::Drivetrain(uint8_t const left_back_motor_port, uint8_t const right_back_motor_port, uint8_t const left_front_motor_port, uint8_t const right_front_motor_port)
	: m_left_back_motor{ left_back_motor_port }
	, m_right_back_motor{ right_back_motor_port }
	, m_left_front_motor{ left_front_motor_port }
	, m_right_front_motor{ right_front_motor_port } {}

void Drivetrain::update(int32_t forward_backward_axis, int32_t left_right_axis) {
	switch (m_reference_frame) {
		case DrivetrainReferenceFrame::Normal:
			break;
		case DrivetrainReferenceFrame::Reversed:
			forward_backward_axis *= -1;
			left_right_axis *= -1;
			break;
	}

	m_left_back_motor.move_velocity(forward_backward_axis + left_right_axis);
	m_left_front_motor.move_velocity(forward_backward_axis + left_right_axis);

	m_right_back_motor.move_velocity(forward_backward_axis - left_right_axis);
	m_right_front_motor.move_velocity(forward_backward_axis - left_right_axis);
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
