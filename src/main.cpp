#include "pros/rtos.hpp"
#include "pros/vision.hpp"

#include "Drivetrain.hpp"
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

void spin_rollers_v2(std::shared_ptr<OdomChassisController> a_odom, Roller a_roller) {

	a_roller.spin_wheel(true); 
	a_odom->moveDistance(0.3333_ft); // 4 in
	
	// TODO: change this line based on how long it needs to spin for
	pros::delay(900); 

	a_roller.spin_wheel(false); 
	a_odom->moveDistance(-0.3333_ft); // 4in
}
void purple_rollers_high(std::shared_ptr<OdomChassisController> a_odom, Drivetrain drivetrain, Flywheel a_flywheel, Roller a_roller) {
	// Do rollers
	spin_rollers_v1(drivetrain, a_roller);
	// Turn on flywheel
	a_flywheel.toggle_active(true);
	// Move backward 3 TILES - 42 cm
	a_odom->moveDistance(-1 * 3_ft * TILE_LENGTH_FT - 1.37795_ft);
	// Turn 90 clockwise
	a_odom->turnAngle(90_deg);
	// Forward 1 tile
	a_odom->moveDistance(1_ft * TILE_LENGTH_FT);
	// Turn 135 counter clockwise
	a_odom->turnAngle(-135_deg);
	// Shoots the disks out of the flywheel twice, 5.5 seconds apart
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
	a_odom->moveDistance(0.25_ft + 1.5_ft * TILE_LENGTH_FT);
	// Shoot low goals
	low_goals(2, a_flywheel);
}

void pink_double_rollers(std::shared_ptr<OdomChassisController> a_odom, Drivetrain a_drivetrain, Roller a_roller) {
	// Rollers
	spin_rollers_v1(a_drivetrain, a_roller);
	// moves back 1 in + 1/2 tile
	a_odom->moveDistance((TILE_LENGTH_FT * 1_ft + 0.08333_ft));
	// 90 deg clockwise
	a_odom->turnAngle(90_deg);
	// 1 + 1/2 tiles
	a_odom->moveDistance(1.5_ft * TILE_LENGTH_FT);
	// Rollers
	spin_rollers_v1(a_drivetrain, a_roller);
}

void autonomous() {
	auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(-ports::LEFT_BACK_MOTOR), okapi::Motor(-ports::LEFT_FRONT_MOTOR)}), 
								   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));
	// Define parts to be manipulated
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, 
	ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, 
	ports::RIGHT_FRONT_MOTOR };
	Flywheel  flywheel{ ports::FLYWHEEL_LEFT, 
	ports::FLYWHEEL_RIGHT, ports::PISTON_INDEXER };
	Roller roller {ports::INTAKE_LEFT, ports::INTAKE_RIGHT};


	// purple_rollers_high(odometry, drivetrain, flywheel, roller);
	// green_rollers_low(odometry, drivetrain, flywheel, roller);
	// pink_double_rollers(odometry, drivetrain, roller);

	// // TODO: find time needed to flip rollers
	// roller.spin_wheel(true);
	// pros::Task::delay(4000);
	// roller.spin_wheel(false);



}





