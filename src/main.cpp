#include "pros/rtos.hpp"

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

double const TILE_LENGTH = 1.8266667; // feet
double const ROLLER_LENGTH = TILE_LENGTH - 13.46;

void initialize() {}
void disabled() {}
void competition_initialize() {}

void opcontrol() {
	Drivetrain const drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Flywheel const flywheel{ ports::FLYWHEEL, ports::PISTON_INDEXER };
	Intake const intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	Expansion const expansion{ ports::EXPANSION_PISTON_LEFT, ports::EXPANSION_PISTON_RIGHT};
	Roller const roller { ports::ROLLER };
	Robot robot{ drivetrain, flywheel, intake, expansion, roller};

	while (true) {
		robot.update();
		pros::Task::delay(1);
	}
}


void low_goals(int disk_num, Flywheel a_flywheel) {
	a_flywheel.aim(2);
	pros::Task::delay(5000);

	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(3500);		
	}
	a_flywheel.disengage();

}




void spin_rollers_v1(std::shared_ptr<OdomChassisController> a_odom, Roller a_roller) {
	// Moves bot forward at fastest speed
	a_odom->moveDistance(0.2_ft);

	a_roller.fine_adjust(Roller::FORWARD);
	
	a_odom->moveDistance(-0.2_ft);
 
}

void spin_rollers_v1(std::shared_ptr<OdomChassisController> a_odom) {
	// Moves bot forward at fastest speed

	pros::Motor roller = pros::Motor(ports::ROLLER);
	roller.move_velocity(115);	
	a_odom->moveDistance(0.2_ft);
	pros::Task::delay(400);
	roller.move_velocity(0);	
	a_odom->moveDistance(-0.2_ft);
 
}


// void purple_rollers_high(std::shared_ptr<OdomChassisController> a_odom, Drivetrain drivetrain, Flywheel a_flywheel, Roller a_roller) {
// 	// Do rollers
// 	spin_rollers_v1(a_odom, a_roller);
// 	// Turn on flywheel
// 	a_flywheel.aim(2);
// 	// Move backward 3 TILES - 42 cm
// 	// a_odom->moveDistance(-1 * 3_ft * TILE_LENGTH - 1.37795_ft);
// 	// Turn 90 clockwise
// 	a_odom->turnAngle(90_deg);
// 	// Forward 1 tile
// 	// a_odom->moveDistance(1_ft * TILE_LENGTH);
// 	// // Turn 135 counter clockwise
// 	a_odom->turnAngle(-135_deg);
// 	// // Shoots the disks out of the flywheel twice, 5.5 seconds apart
// 	a_flywheel.shoot();
// 	pros::Task::delay(5500);
// 	a_flywheel.shoot();
// 	pros::Task::delay(20);
// }

void green_rollers_low(std::shared_ptr<OdomChassisController> a_odom, Drivetrain a_drivetrain, Flywheel a_flywheel, Roller a_roller) {
	// Do rollers
	spin_rollers_v1(a_odom, a_roller);
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
	spin_rollers_v1(a_odom, a_roller);
	// moves back 1 in + 1/2 tile
	a_odom->moveDistance((TILE_LENGTH * 1_ft + 0.08333_ft));
	// 90 deg clockwise
	a_odom->turnAngle(90_deg);
	// 1 + 1/2 tiles
	a_odom->moveDistance(1.5_ft * TILE_LENGTH);
	// Rollers
	spin_rollers_v1(a_odom, a_roller);
}




void purple_roller_high_goals(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, int disk_num) {
	
	spin_rollers_v1(odometry);

	pros::Task::delay(300);

	odometry->moveDistance(-0.7_ft);

	odometry->turnAngle(155_deg);
 
	a_flywheel.aim(2);
	
    odometry->moveDistance(3.35_ft * TILE_LENGTH);
	
    // odometry->moveDistance(3.35410197_ft);
	odometry->setMaxVelocity(160.0); // use brain to see the velocity of the flywheel motor at the two diff voltages
    odometry->turnAngle(-93_deg);
	
	for (int i = 0 ; i < (disk_num) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(2500);		
	}
	a_flywheel.disengage();
}

void skills(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, int disk_num) {
	purple_roller_high_goals(odometry, a_flywheel, drivetrain, roller, 3);
	odometry->turnAngle(95_deg);
	odometry->moveDistance(3.66555128_ft * TILE_LENGTH);
	odometry->turnAngle(-45_deg);
	spin_rollers_v1(odometry); 


}

void other_roller (std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, int disk_num) {
	
	odometry->moveDistance(1_ft * TILE_LENGTH);
	odometry->turnAngle(-95_deg);
	spin_rollers_v1(odometry, roller);
	odometry->turnAngle(90_deg);
	a_flywheel.aim(0);
	odometry->moveDistance(-2_ft * TILE_LENGTH);
	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(500);		
	}

}	

void high_goal_sole(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	// flywheel facing front
	intake.toggle(false);
	odometry->moveDistance(-3_ft * TILE_LENGTH);
	a_flywheel.aim(2);
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
	spin_rollers_v1(odometry, roller);

	odometry->moveDistance(-1_ft * TILE_LENGTH);
	odometry->turnAngle(45_deg);
	odometry->moveDistance(-2.8284271_ft * TILE_LENGTH); // sqrt 8
	odometry->turnAngle(-90_deg);

	// call flywheel
	for (int i = 0 ; i < (3); i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1500); 
	}
}

void autonomous() {
    auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(ports::LEFT_BACK_MOTOR), okapi::Motor(ports::LEFT_FRONT_MOTOR)}), 
                                   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));
    Flywheel a_flywheel{ ports::FLYWHEEL, ports::PISTON_INDEXER };
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Roller  roller{ ports::ROLLER };
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };

	// odometry->setMaxVelocity(100);
	purple_roller_high_goals(odometry, a_flywheel, drivetrain, roller, 3);
	// other_roller(odometry, a_flywheel, drivetrain, roller, 3);
	// spin_rollers_v1(odometry);
}