// Main declaration header for NXTPlotter

#pragma debuggerWindows("joystickSimple");
#include "JoystickDriver.c"

#define XAXIS motorA
#define YAXIS motorB
#define TOOLMOTOR motorC

#define XENDSTOP S1
#define YENDSTOP S2

const float mmPerDeg = 0.32; // Distance per encoder count.
const float xMax = 230;
const float yMax = 300;

const int maxPower = 25;
