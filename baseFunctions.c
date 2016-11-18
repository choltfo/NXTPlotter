
#ifndef CONSTANTSH
#include "constants.h"
#endif

void resetAxis(int axis) {
	nMotorEncoder[axis] = 0;
}

float getCurrentAxis(tMotor axis) {
	return nMotorEncoder[axis]*mmPerDeg;
}

int checkEndStop (tMotor axis) {
	return SensorValue[axis == XAXIS? XENDSTOP : YENDSTOP];
}

// Couldn't find this function, but it really should already exist.
float max(float a, float b) {
	return (b>a)?b:a;
}

float min(float a, float b) {
	return (b<a)?b:a;
}

// G1 - move the tool in a straight line.
void moveLinear (float x, float y) {
	motor[XAXIS] = 0;
	motor[YAXIS] = 0;

	float deltaX = x - getCurrentAxis(XAXIS);
	float deltaY = y - getCurrentAxis(YAXIS);

	float magnitude = sqrt(deltaX*deltaX+deltaY*deltaY);

	// http://help.robotc.net/WebHelpArduino/index.htm#page=NXT_Functions_New/NXT_Functions_Motors.htm
	//nMotorEncoderTarget[XAXIS] = x/mmPerDeg;
	//nMotorEncoderTarget[YAXIS] = y/mmPerDeg; //Doesn't work!

	// Is this right?
	//motor[XAXIS] = maxPower*deltaX/magnitude;
	//motor[YAXIS] = maxPower*deltaY/magnitude;

	if (abs(deltaX) > abs(deltaY)) {
		// Closer to 0 or 180 degrees:
		// Therefore, X is the primary axis.
		nSyncedMotors = synchAB;
		nSyncedTurnRatio = 100*(deltaY/deltaX);
		motor[XAXIS] = maxPower*sgn(deltaX);
	} else {
		// Closer to 90 or 270 degrees:
		nSyncedMotors = synchBA;
		nSyncedTurnRatio = 100*(deltaX/deltaY);
		motor[YAXIS] = maxPower*sgn(deltaY);
	}

	while(nMotorRunState[XAXIS] != runStateIdle || nMotorRunState[YAXIS] != runStateIdle) {
		if (getCurrentAxis(XAXIS)*sgn(deltaX) > x*sgn(deltaX))
			motor[XAXIS] = 0;
		if (getCurrentAxis(YAXIS)*sgn(deltaY) > y*sgn(deltaY))
			motor[YAXIS] = 0;
		displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
		displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
	}



	motor[XAXIS] = 0;
	motor[YAXIS] = 0;
	nSyncedMotors = synchNone;
}

// G0 - move the pen as quickly as allowed, with a dogleg.
void moveImmediate (float x, float y) {
	motor[XAXIS] = 0;
	motor[YAXIS] = 0;

	float deltaX = x - getCurrentAxis(XAXIS);
	float deltaY = y - getCurrentAxis(YAXIS);

	motor[XAXIS] = maxPower*sgn(deltaX);
	motor[YAXIS] = maxPower*sgn(deltaY);

	while(nMotorRunState[XAXIS] != runStateIdle || nMotorRunState[YAXIS] != runStateIdle) {
		if (getCurrentAxis(XAXIS)*sgn(deltaX) > x*sgn(deltaX))
			motor[XAXIS] = 0;
		if (getCurrentAxis(YAXIS)*sgn(deltaY) > y*sgn(deltaY))
			motor[YAXIS] = 0;
		displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
		displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
	}



	motor[XAXIS] = 0;
	motor[YAXIS] = 0;
	nSyncedMotors = synchNone;
}

void setToolRotor (int location) {
	location += 3600;
	int curLoc = nMotorEncoder[TOOLMOTOR]%360+3600;
	int delta = location - curLoc;
	delta %= 360;
	motor[TOOLMOTOR] = delta > 0 ? 50 : -50;
	while (nMotorEncoder(TOOLMOTOR)*sgn(delta) > location*sgn(delta)) {}
}

void setTool (int toolNumber) {
		float X = getCurrentAxis(XAXIS);
		float Y = getCurrentAxis(YAXIS);
		nMotorEncoderTarget[TOOLMOTOR] = nMotorEncoder[TOOLMOTOR] + 45;
		motor[TOOLMOTOR] = 25; // Should auto cut off.
		while (nMotorRunState[TOOLMOTOR] != runStateIdle) {}
		moveImmediate(0,0);
		//nMotorEncoderTarget[TOOLMOTOR] = Location of new tool, + 45;
		moveImmediate(X,Y);
		//nMotorEncoderTarget[TOOLMOTOR] = Location of new tool;
}
