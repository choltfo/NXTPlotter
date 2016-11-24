#include "constants.h"

void resetAxis(tMotor axis) {
    nMotorEncoder[axis] = 0;
}

void setAxis (tMotor axis, float value) {
    nMotorEncoder[axis] = value/mmPerDeg;
}

int getTool () {
    int stable = sgn(abs(nMotorEncoder[TOOLMOTOR]) - 60 ); //Positive if above 60 deg.
    if (abs(nMotorEncoder[TOOLMOTOR]) < 30) return 0; // Neutral state is zero.
    int tool = nMotorEncoder[TOOLMOTOR] < 0 ? 2 : 1;

	return stable*tool;
}

float getCurrentAxis(tMotor axis) {
    return nMotorEncoder[axis]*mmPerDeg - ((getTool() == 2) ? toolSeperation : 0);
}

int checkEndStop (tMotor axis) {
    if (axis == YAXIS) {
	    return SensorValue[YENDSTOP] == 6; // White
	} else {
	    return SensorValue[XENDSTOP]; // Pressed
	}
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

void setTool (int toolNumber) {
    if (getTool() == toolNumber) return;

    float curX = getCurrentAxis(XAXIS);
    float curY = getCurrentAxis(YAXIS);

    if (toolNumber == 0) {
        motor[TOOLMOTOR] = abs(getTool()) == 2 ? 50 : -50;
        while (abs(nMotorEncoder[TOOLMOTOR]) > 20) {}
    }
    setTool(0); // Retract tool, then change offset, then change tool.
    playSound(soundBeepBeep);
    moveImmediate(curX,curY); // Move to the position that puts the tool where it should be.

    if (toolNumber == 2) {
        motor[TOOLMOTOR] = -50;
        while (getTool() != toolNumber) {}
    }
    if (toolNumber == 1) {
        motor[TOOLMOTOR] = 50;
        while (getTool() != toolNumber) {}
    }
    motor[TOOLMOTOR] = 0;
}

// Calibrates a single axis (if true calibrate the x-axis, false calibrate the y-axis)
void calibrateAxis (bool xAxisMotor)
{
	if (xAxisMotor){
		motor[XAXIS] = maxPower/2;
		while (!checkEndStop(XAXIS)){}
		motor[XAXIS] = 0;
		setAxis(XAXIS,xMax);
	} else {
		motor[YAXIS] = maxPower/2;
		while (!checkEndStop(YAXIS)){}
		motor[YAXIS] = 0;
		setAxis(YAXIS,yMax);
	}
}
// Calibrates both axes
void calibrate ()
{
	SensorType[XENDSTOP] = sensorTouch;
	SensorType[YENDSTOP] = sensorColorNxtFULL;
	motor[XAXIS] = maxPower/2; // Somewhat slowly, to preserve precision.
	motor[YAXIS] = maxPower/2;
	while (!checkEndStop(XAXIS) && !checkEndStop(YAXIS)){}
	if (checkEndStop(YAXIS)){
		motor[YAXIS] = 0;
		setAxis(YAXIS,yMax);
		calibrateAxis (true);
	}	else {
		motor[XAXIS] = 0;
		setAxis(XAXIS,xMax);
		calibrateAxis (false);
	}
	moveImmediate(-xMax,-yMax);
    resetAxis(XAXIS);
    resetAxis(YAXIS);
}
