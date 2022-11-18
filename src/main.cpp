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
#include <stdio.h>

#include <iostream>
#include <fstream>



double const TILE_LENGTH = 1.8266667; // feet
double const ROLLER_LENGTH = TILE_LENGTH - 13.46;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void test_spin(Drivetrain a_drivetrain, pros::Controller a_controller, Flywheel a_flywheel) {
	std::vector<double> velocities[5000];
	// int targetVelocities[5000][4];
	
	int count = 0;
	// record time
	uint64_t res1 = timeSinceEpochMillisec();
	// while true
	while (true) {
		// spin bot in circle
		a_drivetrain.update(107, 107);

		// inside loop: if button b pressed: break out of loop
		if (a_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			break;
	}
	}

	// while (true) {
	a_flywheel.spin_up();
	a_drivetrain.update(0, 0);

	// record time, find difference
	uint64_t res2 = timeSinceEpochMillisec();
	while (true) {
		
		if (count == 2000) {
			a_drivetrain.update(0, 0);
			a_controller.print(0,0, "TimeZ: %d", (res2-res1));	
			break;
		}
		// print that to the cortex and controller
			
		// pros::Task::delay(1);
		// velocities.push(a_drivetrain.motor_velocities() );
		// targetVelocities.push(a_drivetrain.get_target_velocities())
		
		
		a_controller.print(1,0, "count: %d", count);

		if (count % 5 == 0) {
			// a_controller.print(1,0, "count: %d", a_flywheel.get_voltage() );
			break;
		}	
		count++;
		
	}

	// std::string full ="";
	// for (int i = 0; i < 5000; i++) {
 	// 	full += std::to_string(velocities[i]) + ",";
	// }

	// std::ofstream myfile;
	// myfile.open ("src/example.txt");
	// // myfile << full;
	// myfile << "hi " ;
	// myfile.close();

	
}

double secondsPerAngle(double angle) {
	return 1.17 * (angle/360) * 1000;  // 1.75 seconds/360 degrees UPDATED (*2/3)
}

double millisecondsPerAngle2(double angle) {
	return 11 * angle;
}

void moveAngle(double angle, Drivetrain a_drivetrain, int8_t direction = 1) {

	a_drivetrain.update(0, 90);
	pros::Task::delay(millisecondsPerAngle2(angle)); 
	a_drivetrain.update(0,0);


}

void moveInches(double inches, Drivetrain drivetrain, int8_t direction = 1) { // only accurate after 24in
	// Math:
	// 2 * pi * r = pi * d = length spun in 1.75 seconds
	// d = sqrt(16.5354^2 + 18^2) = 24.4421655 EXTRA COMMENT -> DID I *2
	// inches per 1.75 seconds (circumference of the circles being spun) = 76.7873276
	// the "/" means "per" --> (ex) "1.75 seconds per 76.~~~~ inches"
	// 1.75s / 76.7873276in = 0.0227902188s / 1 in = 1 in / 0.0227902188 s	
	// first movement: TILE_LENGTH * 3 - 16.5354 
	// in seconds: 1 in * (TILE_LENGTH * 3 - 16.5354 ) / 0.0227902188 s	* (TILE_LENGTH * 3 - 16.5354)
	// first movement (in)  = 127 velocity for 1.1218394 seconds

	// UPDATE: multiplied inches per second by 2/3 bc after testing we saw that it was moving
	// 3 feet instead of two feet
	drivetrain.update(127 * direction, 0);
	pros::Task::delay(0.0151934792 * inches * 1000);
	drivetrain.update(0,0);
	return;
}

void spin_rollers(Drivetrain a_drivetrain) {
	// moves bot forward at fastest speed
	a_drivetrain.update(127, 0); 
	pros::Task::delay(1000);

	// drivetrain stops moving
	a_drivetrain.update(0, 0); 

	pros::Task::delay(10); // transition phase

	// moves the drivetrain backwards for 0.1 seconds
	a_drivetrain.update(-127,0); 
	pros::Task::delay(100);

	// stops moving drivetrain
	a_drivetrain.update(0,0); 

	return;
}

void roller_high_goals(Roller a_roller, Flywheel a_flywheel, Drivetrain a_drivetrain) {

	// Do rollers
	spin_rollers(a_drivetrain);

	// Move backward 6 in
	moveInches(20, a_drivetrain, -1);


	moveAngle(135, a_drivetrain, 1);

	// Turns on flywheel, flywheel begins preparing
	a_flywheel.toggle_active(true, false);

	// Move forward 3.2 TILES (width of bot)
	moveInches(TILE_LENGTH * 3.2, a_drivetrain, 1);

	moveAngle(45, a_drivetrain, -1);

	// Shoots the disks out of the flywheel twice, 5.5 seconds apart
	a_flywheel.shoot();
	pros::Task::delay(4500);
	a_flywheel.shoot();
	pros::Task::delay(20);

	// Flywheel turns off
	a_flywheel.disengage();
}


void low_goals(int disk_num, Flywheel a_flywheel) {
	a_flywheel.toggle_active(true, true);
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
	Roller const roller {ports::INTAKE_LEFT, ports::INTAKE_RIGHT, ports::INTAKE_UP};

	Robot robot{ drivetrain, flywheel, intake, expansion, roller};

	pros::Controller t_controller( pros::E_CONTROLLER_MASTER );

	// test_spin(drivetrain, t_controller, flywheel);
	bool flyie = false;
	while (true) {
		flyie = robot.update();
		pros::Task::delay(1);
		t_controller.print(0, 0, "flyie: %d", flyie);
		t_controller.print(0, 0, "YOOOO");

	}
}



void spin_rollers_v1(Drivetrain a_drivetrain, Roller a_roller) {
	// Moves bot forward at fastest speed
	a_drivetrain.update(127, 0); 
	pros::Task::delay(500);

	// Drivetrain stops moving
	a_drivetrain.update(0, 0); 

	// Spin rollers
	a_roller.switch_color();
	
	pros::Task::delay(10); // transition phase

	// Moves the drivetrain backwards for 0.1 seconds
	a_drivetrain.update(-127,0); 
	pros::Task::delay(100);

	// Stops moving drivetrain
	a_drivetrain.update(0,0); 
}


void purple_rollers_high(std::shared_ptr<OdomChassisController> a_odom, Drivetrain drivetrain, Flywheel a_flywheel, Roller a_roller) {
	// Do rollers
	spin_rollers_v1(drivetrain, a_roller);
	// Turn on flywheel
	a_flywheel.toggle_active(true, false);
	// Move backward 3 TILES - 42 cm
	// a_odom->moveDistance(-1 * 3_ft * TILE_LENGTH - 1.37795_ft);
	// Turn 90 clockwise
	a_odom->turnAngle(90_deg);
	// Forward 1 tile
	// a_odom->moveDistance(1_ft * TILE_LENGTH);
	// // Turn 135 counter clockwise
	// a_odom->turnAngle(-135_deg);
	// // Shoots the disks out of the flywheel twice, 5.5 seconds apart
	a_flywheel.shoot();
	pros::Task::delay(5500);
	a_flywheel.shoot();
	pros::Task::delay(20);
}

void green_rollers_low(std::shared_ptr<OdomChassisController> a_odom, Drivetrain a_drivetrain, Flywheel a_flywheel, Roller a_roller) {
	// Do rollers
	spin_rollers_v1(a_drivetrain, a_roller);
	// 5 in back
	a_odom->moveDistance(0.41667_ft);
	// 90 deg clockwise
	a_odom->turnAngle(90_deg);
	// 3 in + 1.5tiles
	a_odom->moveDistance(0.25_ft + 1.5_ft * TILE_LENGTH);
	// Shoot low goals
	low_goals(2, a_flywheel);
}

void pink_double_rollers(std::shared_ptr<OdomChassisController> a_odom, Drivetrain a_drivetrain, Roller a_roller) {
	// Rollers
	spin_rollers_v1(a_drivetrain, a_roller);
	// moves back 1 in + 1/2 tile
	a_odom->moveDistance((TILE_LENGTH * 1_ft + 0.08333_ft));
	// 90 deg clockwise
	a_odom->turnAngle(90_deg);
	// 1 + 1/2 tiles
	a_odom->moveDistance(1.5_ft * TILE_LENGTH);
	// Rollers
	spin_rollers_v1(a_drivetrain, a_roller);
}







void purple_roller_high_goals(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, int disk_num) {
	spin_rollers_v1(drivetrain, roller);


	odometry->moveDistance(-0.5_ft);

	odometry->turnAngle(135_deg);
 
	a_flywheel.toggle_active(true, false);
	
    odometry->moveDistance(3.35_ft * TILE_LENGTH);
	
    // odometry->moveDistance(3.35410197_ft);
    odometry->turnAngle(90_deg);
	
	// for (int i = 0 ; i < (disk_num+1) ; i ++) {
	// 	a_flywheel.shoot();
	// 	pros::Task::delay(3500);		
	// }
	// a_flywheel.disengage();
}


void other_roller (std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, int disk_num) {
	
	odometry->moveDistance(1_ft * TILE_LENGTH);
	odometry->turnAngle(-95_deg);
	spin_rollers_v1(drivetrain, roller);
	// pros::Task::delay(500);
	odometry->turnAngle(90_deg);
	a_flywheel.toggle_active(true, true);
	odometry->moveDistance(-2_ft * TILE_LENGTH);
	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1500);		
	}

}	

void high_goal_sole(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	// flywheel facing front
	intake.toggle(false);
	odometry->moveDistance(-3_ft * TILE_LENGTH);
	a_flywheel.toggle_active(true, false);
	intake.toggle(false);
	odometry->turnAngle(50_deg);
	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1500); 
	}
	intake.toggle(false);
	odometry->moveDistance(1.291_ft);
	intake.toggle(false);
	odometry->moveDistance(-1.291_ft);
	a_flywheel.shoot();	

}

// starting at the roller where we can't directly touch it
void high_goal_roller_indirect(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake) {
	odometry->moveDistance(1_ft * TILE_LENGTH);
	odometry->turnAngle(-90_deg);
	spin_rollers_v1(drivetrain, roller);

	odometry->moveDistance(-1_ft * TILE_LENGTH);
	odometry->turnAngle(45_deg);
	odometry->moveDistance(-2.8284271_ft * TILE_LENGTH); // sqrt 8
	odometry->turnAngle(-90_deg);
	// call flywheel
}

void autonomous() {
    auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(ports::LEFT_BACK_MOTOR), okapi::Motor(ports::LEFT_FRONT_MOTOR)}), 
                                   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));
    Flywheel a_flywheel{ ports::FLYWHEEL_LEFT, ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Roller  roller{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT, ports::INTAKE_UP };
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };

	// odometry->setMaxVelocity(100);
	// green_roller_low_goals(odometry, a_flywheel, drivetrain, roller, 3);
	// other_roller(odometry, a_flywheel, drivetrain, roller, 3);
	high_goal_sole(odometry, a_flywheel, drivetrain, roller, intake, 3);
   
}
 







