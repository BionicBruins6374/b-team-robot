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

#include <chrono>
// #include <Windows.h>
#include <cstdint>


double const TILE_LENGTH = 21.92;
double const ROLLER_LENGTH = TILE_LENGTH - 13.46;



double secondsPerAngle(double angle) {
	return 1.17 * (angle/360) * 1000;  // 1.75 seconds/360 degrees UPDATED (*2/3)
}

double millisecondsPerInch(double inches) {
	// Math:
	// 2 * pi * r = pi * d = length spun in 1.75 seconds
	// d = sqrt(16.5354^2 + 18^2) = 24.4421655
	// inches per 1.75 seconds (circumference of the circles being spun) = 76.7873276
	// the "/" means "per" --> (ex) "1.75 seconds per 76.~~~~ inches"
	// 1.75s / 76.7873276in = 0.0227902188s / 1 in = 1 in / 0.0227902188 s	
	// first movement: TILE_LENGTH * 3 - 16.5354 
	// in seconds: 1 in * (TILE_LENGTH * 3 - 16.5354 ) / 0.0227902188 s	* (TILE_LENGTH * 3 - 16.5354)
	// first movement (in)  = 127 velocity for 1.1218394 seconds

	// UPDATE: multiplied inches per second by 2/3 bc after testing we saw that it was moving
	// 3 feet instead of two feet
	return 0.0151934792 * inches * 1000;
}


void low_goals(int disk_num, Flywheel a_flywheel) {
	a_flywheel.toggle_active(true);
	pros::Task::delay(5000);

	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(3500);		
	}
	a_flywheel.disengage();

}
void initialize() {
	int x_addent = 0;
	int y_addent = 100;
	for (int i =0; i <3; i++) {
		pros::screen::set_pen(COLOR_RED);
		pros::screen::draw_rect(5 + x_addent, 200, 25+x_addent, 150);
		pros::screen::set_pen(COLOR_BLUE);
		pros::screen::draw_rect(5 + x_addent, 100, 25+x_addent, 50);
		x_addent += 20;
	}
}
void disabled() {}
void competition_initialize() {
	// or make a list where each sublist is a line and each item inside the sublist is a function and its corresponding text
	// use lambdas for input where u change from -1 to 1 RO JUST have a function that'll change it based ont eh value
	// (like a switch)
	int x_addent = 0;
	int y_addent = 100;
	for (int i =0; i <3; i++) {
		pros::screen::set_pen(COLOR_RED);
		pros::screen::draw_rect(5 + x_addent, 200, 25+x_addent, 150);
		pros::screen::set_pen(COLOR_BLUE);
		pros::screen::draw_rect(5 + x_addent, 100, 25+x_addent, 50);
		x_addent += 20;
	}
	// Add text for each button later, depending on number of rectangles
}

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
	
	double const ROLLER_LENGTH = TILE_LENGTH - 13.46;
	
	// Define parts to be manipulated
	pros::Motor left_front(ports::LEFT_FRONT_MOTOR);
	pros::Motor right_front(ports::RIGHT_FRONT_MOTOR);
	pros::Motor left_back(ports::LEFT_BACK_MOTOR);
	pros::Motor right_back(ports::RIGHT_BACK_MOTOR);
	// pros::Motor_Group drivetrain({left_front, left_back, right_front, right_back});
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
	
	// roller_high_goals(roller, flywheel, drivetrain);
	drivetrain.update(127,0); // real value: 107.115948
	pros::Task::delay(millisecondsPerInch(5));
	drivetrain.update(0,0);

	drivetrain.update(0, 127);
	pros::Task::delay(secondsPerAngle(90)); 
	drivetrain.update(0,0);


// 	drivetrain.update(0, -127);
// 	pros::Task::delay(secondsPerAngle(90)); 
// 	drivetrain.update(0,0);
}




