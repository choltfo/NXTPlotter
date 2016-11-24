// Main declaration header for NXTPlotter

#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma debuggerWindows("joystickSimple");
#include "JoystickDriver.c"

#define XAXIS motorA
#define YAXIS motorB
#define TOOLMOTOR motorC

#define XENDSTOP S1
#define YENDSTOP S2

const float mmPerDeg = 0.32; // Distance per encoder count.
const float xMax = 200; // Should be 220
const float yMax = 260; // Should be 280

const float toolSeperation = 33.0;

const int maxPower = 20;

#endif
