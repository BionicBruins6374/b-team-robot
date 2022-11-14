#include <cmath>
#include <vector>

#include "Drivetrain.hpp"

#include "constants.hpp"

Drivetrain::Drivetrain(int8_t const left_back_motor_port, int8_t const right_back_motor_port, int8_t const left_front_motor_port, int8_t const right_front_motor_port)
	: m_left_back_motor{ left_back_motor_port, true }
	, m_right_back_motor{ right_back_motor_port }
	, m_left_front_motor{ left_front_motor_port, true }
	, m_right_front_motor{ right_front_motor_port } {}

static float scale(float const raw) {
	return std::pow(raw / constants::CONTROLLER_ANALOG_MAX, 3.0f) * constants::DRIVE_MAX_VELOCITY * constants::DRIVE_DAMPENING;
}

int32_t Drivetrain::polarize(int32_t metric) {
	switch (m_reference_frame) {
		case DrivetrainReferenceFrame::IntakeAtFront:
			return metric;
			break;
		case DrivetrainReferenceFrame::FlywheelAtFront:
			return metric * -1;
			break;
	}
}
void Drivetrain::update(int32_t forward_backward_axis_int, int32_t left_right_axis_int) {
	forward_backward_axis_int = polarize(forward_backward_axis_int);
	left_right_axis_int = polarize(left_right_axis_int);

	auto forward_backward_axis = static_cast<float>(forward_backward_axis_int);
	auto left_right_axis = static_cast<float>(left_right_axis_int);

	forward_backward_axis = scale(forward_backward_axis);
	left_right_axis = scale(left_right_axis);

	auto const left_velocity = static_cast<int32_t>(forward_backward_axis - left_right_axis);
	auto const right_velocity = static_cast<int32_t>(forward_backward_axis + left_right_axis);

	m_left_back_motor.move_velocity(left_velocity);
	m_left_front_motor.move_velocity(left_velocity);

	m_right_back_motor.move_velocity(right_velocity);
	m_right_front_motor.move_velocity(right_velocity);
}

void Drivetrain::next_reference_frame() {
	switch (m_reference_frame) {
		case DrivetrainReferenceFrame::IntakeAtFront:
			m_reference_frame = DrivetrainReferenceFrame::FlywheelAtFront;
			break;
		case DrivetrainReferenceFrame::FlywheelAtFront:
			m_reference_frame = DrivetrainReferenceFrame::IntakeAtFront;
			break;
	}
}

bool Drivetrain::flywheel_front() {
	if (m_reference_frame == DrivetrainReferenceFrame::FlywheelAtFront) {
		return true;
	}
	return false;
}

void Drivetrain::modify_voltage(int16_t voltage) {
	pros::Motor_Group drivetrain ({m_right_back_motor, m_right_front_motor, m_left_back_motor, m_left_front_motor});
	drivetrain.move_voltage(polarize(voltage));
}

std::vector<double> Drivetrain::motor_velocities() {
	pros::Motor_Group drive ({m_right_back_motor, m_right_front_motor, m_left_back_motor, m_left_front_motor});
	return drive.get_actual_velocities();
}
double Drivetrain::get_voltage() {
	pros::Motor motors [4] = {m_right_back_motor, m_right_front_motor, m_left_back_motor, m_left_front_motor};
	double average = 0;
	int8_t motor_num = sizeof(motors) / sizeof(pros::Motor);

	for (int i = 0; i < motor_num; i++) {
		average += motors[i].get_actual_velocity();
	}
	
	return average / motor_num;

}
