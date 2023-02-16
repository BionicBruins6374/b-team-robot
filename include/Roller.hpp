#pragma once

#include <cstdint>
#include "pros/motors.hpp"

class Roller {
private:
	pros::Motor m_motor;
	bool m_on = false;

public:
	enum RollerCode {
		FORWARD = 0,
		BACKWARD = 1,
		STOP = 2
	};

	Roller(int8_t const port);
	void switch_color() const;
	void fine_adjust(RollerCode code);
	void spin_wheel(double scaler) ;	
};