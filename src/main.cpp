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
double const ROBOT_LENGTH = 1.5; // feet
double const ROLLER_LENGTH = 0.1666667; // feet

// double const ROLLER_LENGTH = TILE_LENGTH - 13.46;

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
	pros::Task::delay(1000);

	for (int i = 0 ; i < (disk_num+1) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(100);		
	}
	a_flywheel.disengage();

}



void spin_rollers_v1(std::shared_ptr<OdomChassisController> a_odom, Roller a_roller) {
	// Moves bot forward at fastest speed
	a_odom->moveDistance(0.20_ft);

	a_roller.spin_wheel(1.2);
	pros::Task::delay(270);
	a_roller.spin_wheel(1.1);

	
	a_odom->moveDistance(-0.20_ft);
 
}

void spin_rollers_v1(std::shared_ptr<OdomChassisController> a_odom, Roller a_roller, bool ignore) {
	// Moves bot forward at fastest speed
	a_odom->moveDistance(0.2_ft);

	a_roller.spin_wheel(1.5);
	pros::Task::delay(350);
	a_roller.spin_wheel(1);
	// a_roller.fine_adjust(Roller::BACKWARD);
	// pros::Task::delay(500);
	// a_roller.fine_adjust(Roller::STOP);
	
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

	odometry->turnAngle(145_deg);

	
 
	a_flywheel.aim(1);
	
    odometry->moveDistance(3.35_ft * TILE_LENGTH);
	
    // odometry->moveDistance(3.35410197_ft);
	odometry->setMaxVelocity(160.0); // use brain to see the velocity of the flywheel motor at the two diff voltages
    odometry->turnAngle(-93_deg);
	
	for (int i = 0 ; i < (disk_num) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1000);		
	}
	a_flywheel.disengage();
}


void new_roller_high_goals_side(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	
	// turn on flywheel 
	a_flywheel.aim(3);
	// spin rollers
	spin_rollers_v1(odometry, roller);

	// wait for flywheel to turn on
	pros::Task::delay(350);
	// odometry->turnAngle(-3_deg);
	pros::Task::delay(2500);
	
	// shoot the two preloads
	for (int i = 0 ; i < (2) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1500);		
	}

	// turn off flywheel
	a_flywheel.disengage();

	// turn on intake
	intake.toggle(false);
	
	// turn to face the disk pile, diagonally
	odometry->turnAngle(3.5_deg); 
	// move to disks
	odometry->moveDistance(-1_ft * (TILE_LENGTH - 1.66667));
	// knock over disks & wait
	pros::Task::delay(200);
	odometry->turnAngle(111.39100_deg); 
	odometry->moveDistance(2.9437_ft); 
	odometry->moveDistance(-0.3_ft);
	pros::Task::delay(100);
	odometry->setMaxVelocity(70.0); // use brain to see the velocity of the flywheel motor at the two diff voltages
	odometry->moveDistance(1.3_ft);
	pros::Task::delay(50);
	odometry->turnAngle(-136_deg);
	

}


void corners_start_roller(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	// turn on flywheel 
	a_flywheel.aim(3);
	// spin rollers
	spin_rollers_v1(odometry, roller, true);

	// wait for flywheel to turn on
	pros::Task::delay(350);
	pros::Task::delay(2500);
	
	// shoot the two preloads
	for (int i = 0 ; i < (2) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1300);		
	}

	// go backwards to meet the line
	odometry->moveDistance(TILE_LENGTH * -1_ft);
	// turn towards second roller
	odometry->turnAngle(90_deg);
	// go forward to second roller
	odometry->moveDistance(TILE_LENGTH* 0.9 * 1_ft);
	// spin rollers
	spin_rollers_v1(odometry, roller, true);
}
void rollers_side_skill(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	// turn on flywheel 
	a_flywheel.aim(3);
	// spin rollers
	spin_rollers_v1(odometry, roller, true);

	// wait for flywheel to turn on
	pros::Task::delay(350);
	pros::Task::delay(2500);
	
	// shoot the two preloads
	for (int i = 0 ; i < (2) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1300);		
	}

	// go backwards to meet the line
	odometry->moveDistance(TILE_LENGTH * -1_ft);
	// turn towards second roller
	odometry->turnAngle(-90_deg);
	// go forward to second roller
	odometry->moveDistance(TILE_LENGTH* 0.9 * 1_ft);
	// spin rollers
	spin_rollers_v1(odometry, roller, true);

	// go back to middle line
	odometry->moveDistance(TILE_LENGTH* -0.9 * 1_ft);
	// turn clockwise 45 degrees
	odometry->turnAngle(45_deg);
	// turn on intake
	intake.toggle(false);
	// go forward towards midpoint of field
	odometry->moveDistance(-2.82842712_ft * TILE_LENGTH);
	// turn 90 deg clockwise
	odometry->turnAngle(90_deg);
	// shoot the 3 intake disks
	for (int i = 0 ; i < (3) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1300);		
	}
	// turn so intake facing disks again
	odometry->turnAngle(-90_deg);
	// go forward to a certain point
	odometry->moveDistance(-3.473_ft * TILE_LENGTH);
	// turn to shoot maybeebeeebebeb later
	// turn towards roller
	odometry->turnAngle(45_deg);
	// move forward one tile
	odometry->moveDistance(0.9_ft * TILE_LENGTH);

	spin_rollers_v1(odometry, roller, true);

	odometry->moveDistance(-1_ft * TILE_LENGTH);

	odometry->turnAngle(-90_deg);

	odometry->moveDistance(1.25_ft * TILE_LENGTH);
	spin_rollers_v1(odometry, roller, true);

	

}
void roller_high_goals_side_minchan(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel a_flywheel, Drivetrain drivetrain, Roller roller, Intake intake, int disk_num) {
	
	a_flywheel.aim(2);
	spin_rollers_v1(odometry);

	pros::Task::delay(300);

	// odometry->moveDistance(-0.7_ft);

	odometry->turnAngle(-3.5_deg); // counterclockwise


	
	// a_flywheel.aim(1);
	// a_flywheel.aim(1);
	pros::Task::delay(2500);
    // odometry->moveDistance(3.35_ft * TILE_LENGTH);
	
    // // odometry->moveDistance(3.35410197_ft);
	// odometry->setMaxVelocity(160.0); // use brain to see the velocity of the flywheel motor at the two diff voltages
    // odometry->turnAngle(-93_deg);
	
	for (int i = 0 ; i < (2) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1000);		
	}

	intake.toggle(false);
	odometry->turnAngle(3.5_deg); 
	odometry->moveDistance(-1_ft * (TILE_LENGTH - 1.66667));
	pros::Task::delay(200);
	odometry->turnAngle(111.39100_deg); 
	odometry->moveDistance(2.9437_ft); 
	odometry->moveDistance(-0.3_ft);
	pros::Task::delay(100);
	odometry->setMaxVelocity(70.0); // use brain to see the velocity of the flywheel motor at the two diff voltages
	odometry->moveDistance(0.3_ft);
	pros::Task::delay(50);
	odometry->turnAngle(-136_deg);
	a_flywheel.shoot();
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

void far_roller(std::shared_ptr<okapi::OdomChassisController> odometry, Flywheel flywheel, Drivetrain drivetrain, Roller roller, Intake intake) {
	flywheel.aim(3);
	intake.toggle(false); 
	odometry->moveDistance((TILE_LENGTH - ROBOT_LENGTH + TILE_LENGTH/2 + 0.2) * 1_ft); 
	pros::Task::delay(500);
	intake.toggle(false);
	odometry->turnAngle(90_deg + 24_deg); // 90 - 66 = 24 (check miro)
	odometry->moveDistance(3.54833045_ft);
	odometry->turnAngle(66_deg);
	spin_rollers_v1(odometry, roller);
	
	odometry->turnAngle(45_deg);

	// 2 preload disks + 1 intake disk
	for (int i = 0 ; i < 3 ; i ++) {
		flywheel.shoot();
		pros::Task::delay(1500);		
	}

	flywheel.disengage();
}

void autonomous() {
    auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(ports::LEFT_BACK_MOTOR), okapi::Motor(ports::LEFT_FRONT_MOTOR)}), 
                                   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));
    Flywheel a_flywheel{ ports::FLYWHEEL, ports::PISTON_INDEXER };
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Roller  roller{ ports::ROLLER };
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };

	// odometry->setMaxVelocity(100);
	// new_roller_high_goals_side(odometry, a_flywheel, drivetrain, roller, intake, 2);
	// rollers_side_skill(odometry, a_flywheel, drivetrain, roller, intake, 3);
	spin_rollers_v1(odometry, roller);
	// far_roller(odometry, a_flywheel, drivetrain, roller, intake);
	// other_roller(odometry, a_flywheel, drivetrain, roller, 3);
	// spin_rollers_v1(odometry, roller);
}