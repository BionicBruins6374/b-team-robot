#include "constants.hpp"
#include "pros/misc.h"
#include <iostream>
#include "Flywheel.hpp"
#include "Robot.hpp"


Robot::Robot(Drivetrain drivetrain, Flywheel flywheel, Intake intake, Expansion expansion, Roller roller)
	: m_drivetrain(drivetrain), m_flywheel(flywheel), m_intake(intake), m_expansion(expansion), m_roller(roller) {}



bool Robot::update_flywheel() {

	flywheel_turned_on = false;
	
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		m_flywheel.aim(0);
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		m_flywheel.aim(1);
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
		m_flywheel.shoot();
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
		flywheel_turned_on = m_flywheel.toggle_active(true, true);
	} else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
		flywheel_turned_on = m_flywheel.toggle_active(true, false);
	}
	m_controller.print(5,0, "voltage: %d", 20 );	
	return flywheel_turned_on;
}


bool Robot::update_drivetrain(bool flywheel_on) {
	m_drivetrain.update(m_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), m_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
	// bool flyie = false;
	// if (flywheel_on && !m_drivetrain.flywheel_front()) {
	// 	m_drivetrain.next_reference_frame();
	// 	flyie = true;
	// }

	if (flywheel_on) {
		m_drivetrain.set_front(1);
	}

	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		m_drivetrain.next_reference_frame();
		// flyie = true;
	}
	
	// update later to only change the voltage if the voltage isn't in like a 10 percent range of the specified volts
	double current_volts = m_drivetrain.get_voltage(); 
	if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
		// if (m_controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) && (current_volts * 0.92 <= constants::SLOWER_DRIVETRAIN && constants::SLOWER_DRIVETRAIN <= current_volts * 1.08)) {
		m_drivetrain.modify_voltage(constants::SLOWER_DRIVETRAIN);
	}
	return flywheel_on;
	
}




void Robot::update_intake() {
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
		m_intake.toggle(false);
		m_roller.spin();
	}
	else if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
		m_intake.toggle(true);
	}
}

void Robot::update_expansion() {
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
		m_expansion.trigger();
	}
}

void Robot::update_roller() {
	if (m_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		m_roller.switch_color();
	}
}

bool Robot::update() {
	bool flywheel_pressed_on = update_flywheel();
	bool flyie = update_drivetrain(flywheel_pressed_on);
	update_intake();
	update_expansion();
	update_roller();
	return flyie;
}
