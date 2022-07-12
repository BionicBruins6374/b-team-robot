#include <pros/misc.h>

#include "Robot.hpp"

Robot::Robot(Drivetrain drivetrain, Sensor sensor, Flywheel flywheel, Intake intake, Roller roller)
	: m_drivetrain(drivetrain), m_sensor(sensor), m_flywheel(flywheel), m_intake(intake), m_roller(roller) {}

void Robot::update_drivetrain() {
	m_drivetrain.update(m_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), m_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		m_drivetrain.next_reference_frame();
	}
}

void Robot::update_flywheel() {
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
		auto const dist = m_sensor.get_distance();
		if (dist.has_value()) {
			m_flywheel.aim(dist.value());
		} else {
			m_controller.rumble(".");
		}
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
		m_flywheel.shoot();
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
		m_flywheel.toggle_active();
	}
}

void Robot::update_intake() {
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
		m_intake.toggle();
	}
}

void Robot::update_roller() {
	if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
		m_roller.fine_adjust(false);
	} else if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
		m_roller.fine_adjust(true);
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		m_roller.switch_color();
	}
}

void Robot::update() {
	update_drivetrain();
	update_flywheel();
	update_intake();
	update_roller();
}
