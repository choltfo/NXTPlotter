#include "constants.h"

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

int getTool () {
    int stable = sgn(abs(nMotorEncoder[TOOLMOTOR]) - 60 ); //Positive if above 60 deg.
    if (abs(nMotorEncoder[TOOLMOTOR]) < 30) return 0; // Neutral state is zero.
    int tool = nMotorEncoder[TOOLMOTOR] < 0 ? 2 : 1;

	return stable*tool;
}

void setTool (int toolNumber) {
    if (getTool() == toolNumber) return;
    if (toolNumber == 2) {
        motor[TOOLMOTOR] = -50;
        while (getTool() != toolNumber) {}
    }
    if (toolNumber == 1) {
        motor[TOOLMOTOR] = 50;
        while (getTool() != toolNumber) {}
    }
    if (toolNumber == 0) {
        motor[TOOLMOTOR] = abs(getTool()) == 2 ? 50 : -50;
        while (abs(nMotorEncoder[TOOLMOTOR]) > 20) {}
    }
    motor[TOOLMOTOR] = 0;
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

// Calibrates a single axis (if true calibrate the x-axis, false calibrate the y-axis)
void calibrateAxis (bool xAxisMotor)
{
	if (xAxisMotor){
		motor[XAXIS] = maxPower;
		while (SensorValue[XENDSTOP] != 6){}
		motor[XAXIS] = 0;
		resetAxis(XAXIS);
	} else {
		motor[YAXIS] = maxPower;
		while (SensorValue[YENDSTOP] != 1){}
		motor[YAXIS] = 0;
		resetAxis(YAXIS);
	}
}
// Calibrates both axes
void calibrate ()
{
	SensorType[YENDSTOP] =  sensorTouch;
	SensorType[XENDSTOP] = sensorColorNxtFULL;
	setTool(0);
	motor[XAXIS] = maxPower;
	motor[YAXIS] = maxPower;
	while (SensorValue[YENDSTOP] != 1 && SensorValue[XENDSTOP] != 6){}
	if (SensorValue[YENDSTOP] == 1){
		motor[YAXIS] = 0;
		resetAxis(YAXIS);
		calibrateAxis (true);
	}	else {
		motor[XAXIS] = 0;
		resetAxis(XAXIS);
		calibrateAxis (false);
	}
}
