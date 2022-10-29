#pragma once

extern "C" {
void initialize();
void disabled();
void competition_initialize();
void autonomous();
void opcontrol();
}

double const TILE_LENGTH_IN = 21.92;
double const TILE_LENGTH_FT = 1.8266667;