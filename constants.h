// Final project, group 16
// Deus Ex Machina: a plot device
// November 2016
// RobotC code

// Main declaration header for NXTPlotter

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Jotstick required.
#pragma debuggerWindows("joystickSimple");
#include "JoystickDriver.c"

// Because we were losing track of which was which:
#define XAXIS motorA
#define YAXIS motorB
#define TOOLMOTOR motorC

#define XENDSTOP S1
#define YENDSTOP S2

const float mmPerDeg = 0.32; // Distance per encoder count.

// Size of page.
const float xMax = 250;
const float yMax = 260;

// Distance between tool heads.
const float toolSeperation = 33.0;

const int maxPower = 20;

#endif
