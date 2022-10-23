#include "pros/rtos.hpp"
#include "pros/vision.hpp"

#include "Flywheel.hpp"
#include "Robot.hpp"
#include "Intake.hpp"

#include "main.hpp"
#include "ports.hpp"

#include "odometry.hpp"
#include "Expansion.hpp"

#include "okapi/impl/device/motor/motor.hpp"
#include "okapi/impl/device/motor/motorGroup.hpp"

void initialize() {}
void disabled() {}
void competition_initialize() {}

void opcontrol() {
	Drivetrain const drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Flywheel const flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Intake const intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Expansion const expansion{ ports::EXPANSION_PISTON };
	Roller const roller {ports::INTAKE_LEFT, ports::INTAKE_RIGHT};

	Robot robot{ drivetrain, flywheel, intake, expansion, roller};

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}

void autonomous() {
	auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(-ports::LEFT_BACK_MOTOR), okapi::Motor(-ports::LEFT_FRONT_MOTOR)}), 
								   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));

	double const TILE_LENGTH = 21.92;
	double const ROLLER_LENGTH = TILE_LENGTH - 13.46;
	
	Flywheel m_flywheel (ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER);
	
	// Define parts to be manipulated
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Flywheel  flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Expansion expansion {ports::EXPANSION_PISTON};
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Roller  roller{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };

	// Create roller group based on motors separately in order to do continuous spin 
	pros::Motor roller_left(ports::INTAKE_LEFT);
	pros::Motor roller_right(ports::INTAKE_RIGHT);
	pros::Motor_Group rollers({roller_left, roller_right});

	
	pros::Task::delay(10); // just so that we don't start the second auton does
	drivetrain.update(127, 0); // moves us forward at fastest speed

	// rollers.move_velocity(150); turns on rollers, unnecessary

	pros::Task::delay(1000);
	// rollers.move_velocity(0); turns off rollers, unnecessary
	drivetrain.update(0, 0); // drivetrain stops moving

	pros::Task::delay(10); // transition phase

	// moves the drivetrain backwards for 0.1 seconds
	drivetrain.update(-127,0); 
	pros::Task::delay(100);

	// stops moving drivetrain
	drivetrain.update(0,0); 
	
	// Low goals strategy 
	
	// m_flywheel.toggle_active(true);
	// pros::Task::delay(5000);
	// m_flywheel.shoot();
	// pros::Task::delay(3500);
	// m_flywheel.shoot();
	// m_flywheel.toggle_active(true);


	// Future testing for angle turns

	// motor_group.move_velocity(150);
	// pros::Task::delay(3288);
	// motor_group.move_velocity(0);
	// ^ spin like 5 times and do (5 * 360)/x = 90 ----> t/x = time it takes to spin 90 degrees 




	
}
