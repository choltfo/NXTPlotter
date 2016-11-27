// Final project, group 16
// Deus Ex Machina: a plot device
// November 2016
// RobotC code

// A collection of functions that make the rest of the plotter much easier to write.

#include "constants.h"

// Charles H
// Zeroes encoders.
void resetAxis(tMotor axis) {
    nMotorEncoder[axis] = 0;
}

// Charles H
// Sets encoder values according to a coordinate.
void setAxis (tMotor axis, float value) {
    nMotorEncoder[axis] = value/mmPerDeg;
}

// Charles H
int getTool () {
    int stable = sgn(abs(nMotorEncoder[TOOLMOTOR]) - 60 ); //Positive if above 60 deg.
    if (abs(nMotorEncoder[TOOLMOTOR]) < 30) return 0; // Neutral state is zero.
    int tool = nMotorEncoder[TOOLMOTOR] < 0 ? 2 : 1;

	return stable*tool;
}

// Ian F
// Returns the current location of the active pen's tip relative to the page.
float getCurrentAxis(tMotor axis) {
    return nMotorEncoder[axis]*mmPerDeg - ((abs(getTool()) != 2 && axis == XAXIS) ? toolSeperation : 0);
}

// Matthew G
// Checks whether an endstop (sensor) is pressed/seeing white.
int checkEndStop (tMotor axis) {
    if (axis == YAXIS) {
	    return SensorValue[YENDSTOP] == 6; // White - colourWhite was not working
	} else {
	    return SensorValue[XENDSTOP]; // Pressed
	}
}

// Charles H
// G1 - move the tool in a straight line.
void moveLinear (float x, float y) {
    motor[XAXIS] = 0;
    motor[YAXIS] = 0;

    float deltaX = x - getCurrentAxis(XAXIS);
    float deltaY = y - getCurrentAxis(YAXIS);

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
        displayTextLine(2,"X: %f",getCurrentAxis(XAXIS));
        displayTextLine(3,"Y: %f",getCurrentAxis(YAXIS));
    }



    motor[XAXIS] = 0;
    motor[YAXIS] = 0;
    nSyncedMotors = synchNone;
}

// Charles H
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
        displayTextLine(2,"X: %f",getCurrentAxis(XAXIS));
        displayTextLine(3,"Y: %f",getCurrentAxis(YAXIS));
    }



    motor[XAXIS] = 0;
    motor[YAXIS] = 0;
    nSyncedMotors = synchNone;
}


// Charles H
// Sets the tool currently engaging the paper to 0, 1, or 2.
void setTool (int toolNumber) {
    if (getTool() == toolNumber) return;

    float curX = getCurrentAxis(XAXIS);
    float curY = getCurrentAxis(YAXIS);

    if (toolNumber == 0) {
        motor[TOOLMOTOR] = abs(getTool()) == 2 ? 50 : -50;
        while (abs(nMotorEncoder[TOOLMOTOR]) > 20) {}
        motor[TOOLMOTOR] = 0;
        moveImmediate(curX,curY); // Move to correct position (For tool 2-0 transfer)
    }

    playSound(soundShortBlip);

    moveImmediate(curX-(toolNumber == 2?toolSeperation:0),curY); // Move to the position that puts the tool where it should be.

    if (toolNumber == 2) {
        motor[TOOLMOTOR] = -25;
        while (getTool() != toolNumber) {}
    }
    if (toolNumber == 1) {
        motor[TOOLMOTOR] = 25;
        while (getTool() != toolNumber) {}
    }
    motor[TOOLMOTOR] = 0;
}

// Matthew G
// Calibrates a single axis (if 'true', calibrate the x-axis, false calibrate the y-axis)
void calibrateAxis (bool xAxis) {
	if (xAxis){
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
// Matthew G
void calibrate () {
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
