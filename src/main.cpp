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

#include "okapi/impl/device/rotarysensor/integratedEncoder.hpp"
// #include "okapi/impl/chassis/controller/chassisControllerBuilder.hpp"
// #include "okapi/impl/device/motor/motorGroup.hpp"
// #include <cstdint>

#include "cmath"
#define PI 3.14159265
#include "string"

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

void spin_rollers_v1(std::shared_ptr<OdomChassisController> a_odom, Roller a_roller, double distance, int millisec) {
	// Moves bot forward at fastest speed
	a_odom->moveDistance(1_ft * distance);

	a_roller.spin_wheel(1.2);
	pros::Task::delay(millisec);
	a_roller.spin_wheel(1.1);

	
	a_odom->moveDistance(-1_ft * distance);
 
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
	std::printf( "spin rollers \n" );
	// spin rollers
	try {
	// spin_rollers_v1(odometry, roller, true);
	}
	catch (...) {
		std::printf("an error");
		odometry->setState(okapi::OdomState({0_ft, 0_ft, 0_deg}));

	}
	// wait for flywheel to turn on
	// pros::Task::delay(350);
	std::printf( "flywheel wait \n" );
	pros::Task::delay(2850);
	std::printf( "shoot preloads \n" );
	// shoot the two preloads
	for (int i = 0 ; i < (2) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1300);		
	}

	// go backwards to meet the line
	std::printf( "backwards \n" );
	odometry->moveDistance(TILE_LENGTH * -1_ft);	
	// turn towards second roller
	std::printf( "turn towards second roller \n" );
	odometry->turnAngle(-90_deg);
	// go forward to second roller
	std::printf( "forward towards second roller \n" );
	odometry->moveDistance(TILE_LENGTH* 0.9 * 1_ft);
	// spin rollers
	std::printf( "spin rollers \n" );
	// spin_rollers_v1(odometry, roller, true);

	// go back to middle line
	std::printf( "go back to middle line \n" );
	odometry->moveDistance(TILE_LENGTH* -0.9 * 1_ft);
	// turn clockwise 90 + 45 degrees
	std::printf( "turn 135 deg \n" );
	odometry->turnAngle(-135_deg);
	// turn on intake
	intake.toggle(false);
	// go forward towards midpoint of field
	std::printf( "midpoint of field \n" );
	odometry->moveDistance(2.82842712_ft * TILE_LENGTH);
	// turn 90 deg clockwise
	std::printf( "turn 90 deg \n" );
	odometry->turnAngle(-90_deg);
	// shoot the 3 intake disks
	std::printf( "shoot disks\n" );
	for (int i = 0 ; i < (3) ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1300);		
	}
	// turn so intake facing disks again
	std::printf( "turn to disks \n" );
	odometry->turnAngle(90_deg);
	// go forward to a certain point
	std::printf( "forward to point \n" );
	odometry->moveDistance(2.473_ft * TILE_LENGTH );
	// turn to shoot maybeebeeebebeb later
	// turn towards roller
	std::printf( "turn to roller \n" );
	odometry->turnAngle(-45_deg);
	// move forward ~one tile (up to roller)
	odometry->moveDistance(0.9_ft * TILE_LENGTH);
	pros::Task::delay(50);
	// spin_rollers_v1(odometry, roller, true);
	// go backwards
	odometry->moveDistance(-1_ft * TILE_LENGTH);
	// turn  90 deg counterclockwise to face second roller
	odometry->turnAngle(90_deg);

	odometry->moveDistance(1.25_ft * TILE_LENGTH);
	// spin_rollers_v1(odometry, roller, true);

	

}


void far_roller(std::shared_ptr<okapi::OdomChassisController> a_odom, Flywheel a_flywheel, Drivetrain a_drivetrain, Roller roller, Intake a_intake) {
	a_flywheel.aim(4);
	a_intake.toggle(false); 

	double y_forward = TILE_LENGTH - ROBOT_LENGTH + TILE_LENGTH;
	double x_forward = 1.25 * TILE_LENGTH;
	double diagonal = std::sqrt(std::pow(y_forward, 2) + std::pow(x_forward, 2));
	double angle_to_turn = std::asin(x_forward/diagonal) * 180.0 / PI;
    // moves forward to disk, then moves 0.2 ft more to intake disk TEST FOR INTAKE
	a_odom->moveDistance(y_forward* 1_ft); 
	// pause to intake
    pros::Task::delay(500);
	// a_intake.toggle(false);
    
    // turn so intake faces roller diagonally 
	a_odom->turnAngle(-90_deg - (90 - angle_to_turn) * 1_deg); // 90 - 66 = 24 (check miro)
	// move to roller
    a_odom->moveDistance(diagonal * 1_ft - 0.25_ft);


	// turn to directly face roller
    a_odom->turnAngle(-1_deg * angle_to_turn);
    // spin rollers
	spin_rollers_v1(a_odom, roller, 0.3, 270);
	
	a_intake.toggle(false);

    // turn to shoot
	a_odom->turnAngle(-14_deg);

	// 2 preload disks + 1 intake disk
	for (int i = 0 ; i < 3 ; i ++) {
		a_flywheel.shoot();
		pros::Task::delay(1800);		
	}

	a_flywheel.disengage();
}



using namespace okapi;


void autonomous() {
	pros::Task::delay(100);
    auto odometry = build_odometry(okapi::MotorGroup({okapi::Motor(ports::LEFT_BACK_MOTOR), okapi::Motor(ports::LEFT_FRONT_MOTOR)}), 
                                   okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)}));
    
	// okapi::MotorGroup left_motors = okapi::MotorGroup({okapi::Motor(ports::LEFT_BACK_MOTOR), okapi::Motor(ports::LEFT_FRONT_MOTOR)});
	// okapi::MotorGroup right_motors = okapi::MotorGroup({okapi::Motor(ports::RIGHT_BACK_MOTOR), okapi::Motor(ports::RIGHT_FRONT_MOTOR)});
	// std::shared_ptr<OdomChassisController> odometry  = okapi::ChassisControllerBuilder()
	// 	.withMotors(left_motors, right_motors)
	// 	.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
	// 	.withOdometry() 
	// 	.buildOdometry(); 

	okapi::IntegratedEncoder left_front_encoder = okapi::IntegratedEncoder(ports::LEFT_FRONT_MOTOR);
	okapi::IntegratedEncoder left_back_encoder = okapi::IntegratedEncoder(ports::LEFT_BACK_MOTOR);
	okapi::IntegratedEncoder right_front_encoder = okapi::IntegratedEncoder(ports::RIGHT_FRONT_MOTOR);
	okapi::IntegratedEncoder right_back_encoder = okapi::IntegratedEncoder(ports::RIGHT_BACK_MOTOR);

	std::printf("left back: %f \n", left_back_encoder.get());
	std::printf("right back: %f \n", right_back_encoder.get());
	std::printf("left front: %f \n", left_front_encoder.get());
	std::printf("right front: %f \n", right_front_encoder.get());

	left_front_encoder.reset(); left_back_encoder.reset(); right_front_encoder.reset(); right_back_encoder.reset();

	
	Flywheel a_flywheel{ ports::FLYWHEEL, ports::PISTON_INDEXER };
	Drivetrain  drivetrain{ ports::LEFT_BACK_MOTOR, ports::RIGHT_BACK_MOTOR, ports::LEFT_FRONT_MOTOR, ports::RIGHT_FRONT_MOTOR };
	Roller  roller{ ports::ROLLER };
	Intake  intake{ ports::INTAKE_LEFT, ports::INTAKE_RIGHT };
	std::printf( "yada %s", odometry->getState().str().c_str() );
	odometry->setState(okapi::OdomState({0_ft, 0_ft, 0_deg}));
	// odometry->setMaxVelocity(100);
	// new_roller_high_goals_side(odometry, a_flywheel, drivetrain, roller, intake, 2);
	pros::Task::delay(50);
	rollers_side_skill(odometry, a_flywheel, drivetrain, roller, intake, 3);
	// spin_rollers_v1(odometry, roller);
	// odometry->setMaxVelocity(130);
	// far_roller(odometry, a_flywheel, drivetrain, roller, intake);
	// other_roller(odometry, a_flywheel, drivetrain, roller, 3);
	// spin_rollers_v1(odometry, roller);
}