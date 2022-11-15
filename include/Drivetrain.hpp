#pragma once

#include <cstdint>
#include "pros/motors.hpp"

enum class DrivetrainReferenceFrame {
	IntakeAtFront,
	FlywheelAtFront,
};

class Drivetrain {
private:
	pros::Motor m_left_back_motor;
	pros::Motor m_right_back_motor;
	pros::Motor m_left_front_motor;
	pros::Motor m_right_front_motor;

	pros::Motor motors [4] = {m_right_back_motor, m_right_front_motor, m_left_back_motor, m_left_front_motor};
	int8_t motor_num = sizeof(motors) / sizeof(pros::Motor);
	
	DrivetrainReferenceFrame m_reference_frame = DrivetrainReferenceFrame::IntakeAtFront;
public:
	Drivetrain(int8_t left_back_motor_port, int8_t right_back_motor_port, int8_t left_front_motor_port, int8_t right_front_motor_port);
	int32_t polarize(int32_t metric);
	void update(int32_t forward_backward_axis, int32_t left_right_axis);
	void next_reference_frame();
	
	bool flywheel_front();
	void modify_voltage(int16_t voltage);

	void set_front(int front);

	double get_voltage(); // returns the average voltage between the 4 motors
	std::vector<double> motor_velocities();
};
