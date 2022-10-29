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
#include <cstdint>


double const TILE_LENGTH = 21.92;
double const ROLLER_LENGTH = TILE_LENGTH - 13.46;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void test_spin(Drivetrain a_drivetrain, pros::Controller a_controller) {
	// record time
	uint64_t res1 = timeSinceEpochMillisec();
	while (true) {
		// spin bot in circle
		a_drivetrain.update(107, 107);
		// inside loop: if button b pressed: break out of loop
		if (a_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) { break; }
	}
	a_drivetrain.update(0,0);
	// record time, find difference
	uint64_t res2 = timeSinceEpochMillisec();
	while (true) {
		// print that to the cortex and controller
		a_controller.print(0,0, "Time: %d", (res2-res1));		
	}
}

double ms_per_angle(double angle) {
	return 7 * angle;  // 2523.3 milliseconds per 360 degrees with a_drivetrain.update(107, 107) 
}

double ms_per_inch(double inches) {
	return 0.0151934792 * inches * 1000;
}

void low_goals(int disk_num, Flywheel a_flywheel) {

	// Turns on flywheel but doesn't let it fully 
	// -> it won't shoot accurately/into high goals
	a_flywheel.toggle_active(true);
	pros::Task::delay(2000);

	// Loops through numbers of disks: 
	for (int i = 0 ; i < disk_num ; i ++) {
		// Shoots disk and lets flywheel recover 
		a_flywheel.shoot();
		pros::Task::delay(1500);		
	}
}

void spin_rollers(Drivetrain a_drivetrain) {
	// Moves bot forward at fastest speed
	a_drivetrain.update(127, 0); 
	pros::Task::delay(1000);

	// Drivetrain stops moving
	a_drivetrain.update(0, 0); 

	pros::Task::delay(10); // transition phase

	// Moves the drivetrain backwards for 0.1 seconds
	a_drivetrain.update(-127,0); 
	pros::Task::delay(100);

	// Stops moving drivetrain
	a_drivetrain.update(0,0); 
}

void roller_high_goals_purple(Roller a_roller, Flywheel a_flywheel, Drivetrain a_drivetrain) {
	// Do rollers
	spin_rollers(a_drivetrain);

	// Move backward 3 TILES - 42cm (width of bot)
	double ms_three_tiles = ms_per_inch(TILE_LENGTH * 3 - 16.5354 );
	a_drivetrain.update(-127, 0);
	pros::Task::delay(ms_three_tiles/2);

	// Turns on flywheel, flywheel begins preparing
	a_flywheel.toggle_active(true);

	pros::Task::delay(ms_three_tiles/2);
	a_drivetrain.update(0,0);

	// Turn right 90 degrees, clockwise (right)
	a_drivetrain.update(0, 127);
	pros::Task::delay(ms_per_angle(90)); 
	a_drivetrain.update(0,0);

	// Move forward 1 TILE
	a_drivetrain.update(-127, 0);
	pros::Task::delay(ms_per_inch(TILE_LENGTH)); 

	// Turn left 130 DEGREES
	a_drivetrain.update(0, -127);
	pros::Task::delay(ms_per_angle(135)); // converts to milliseconds
	a_drivetrain.update(0,0);

	// Shoots the disks out of the flywheel twice, 5.5 seconds apart
	a_flywheel.shoot();
	pros::Task::delay(5500);
	a_flywheel.shoot();
	pros::Task::delay(20);
}

void roller_low_goals_green(Roller a_roller, Flywheel a_flywheel, Drivetrain a_drivetrain) {
	// Spin rollers
	spin_rollers(a_drivetrain);

	// Move back 5 in
	a_drivetrain.update(-127, 0);
	pros::Task::delay(ms_per_inch(5));
	a_drivetrain.update(0,0);

	// Turn right 90 degrees
	a_drivetrain.update(0, 107);
	pros::Task::delay(ms_per_angle(90));
	a_drivetrain.update(0,0);

	// Move backwards to low goal for 1 TILE + 1/2 TILE + 3 in
	a_drivetrain.update(-127, 0);
	pros::Task::delay(ms_per_inch(TILE_LENGTH + 0.5 * TILE_LENGTH + 3));
	a_drivetrain.update(0,0);

	// Low goals flywheel shootie
	low_goals(2, a_flywheel);

}

void double_rollers_pink(Roller a_roller, Drivetrain a_drivetrain) {
	// Do rollers
	spin_rollers(a_drivetrain);

	// Move back 1/2 TILE + 1 in
	a_drivetrain.update(-127, 0);
	pros::Task::delay(ms_per_inch(0.5 * TILE_LENGTH + 1));
	a_drivetrain.update(0,0);

	// Turn right 90 degrees
	a_drivetrain.update(0, 107);
	pros::Task::delay(ms_per_angle(90));
	a_drivetrain.update(0,0);

	// Move forward 1 + 1/2 tile
	a_drivetrain.update(127, 0);
	pros::Task::delay(ms_per_inch(0.5 * TILE_LENGTH + TILE_LENGTH));
	a_drivetrain.update(0,0);

	// Do rollers
	spin_rollers(a_drivetrain);
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

	pros::Controller t_controller( pros::E_CONTROLLER_MASTER );

	test_spin(drivetrain, t_controller);

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}

void autonomous() {
	
	// Define parts to be manipulated
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, 
	ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, 
	ports::RIGHT_FRONT_MOTOR };
	Flywheel  flywheel{ ports::FLYWHEEL_LEFT, 
	ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Expansion expansion {ports::EXPANSION_PISTON};
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Roller  roller{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	
	pros::Task::delay(10); // Just so that we don't start the second auton does
	
	spin_rollers(drivetrain);

<<<<<<< HEAD
=======
	odometry->turnAngle(45_deg);
	odometry->moveDistance(0.8333_ft);
>>>>>>> 4e4238f (blah2)
}





