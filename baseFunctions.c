
#include "constants.h"

#include "fileIO.c"

void resetAxis(int axis) {
	nMotorEncoder[axis] = 0;
}

float getCurrentAxis(int axis) {
	return nMotorEncoder[axis]*mmPerDeg;
}

int checkEndStop (int axis) {
	return sensorValue[axis];
}

// Couldn't find this function, but it really should already exist.
float max(float a, float b) {
	return (b>a)?b:a;
}

float min(float a, float b) {
	return (b<a)?b:a;
}

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

void setTool (int toolNumber) {
		motor[TOOLMOTOR] = 100;
}


task main() {
	resetAxis(XAXIS);
	resetAxis(YAXIS);

	moveLinear(100,100);
	wait1Msec(1000);
	moveLinear(-100,100);
	wait1Msec(1000);
	moveLinear(-100,-100);
	wait1Msec(1000);

	TPCJoystick joystick;
	while (true) {
		getJoystickSettings(joystick);
		if(abs(joystick.joy1_y1) > 10) {
      motor[YAXIS] = (joystick.joy1_y1)/100.0*maxPower;
    }
    else                                      // Else if the readings are within the threshold...
    {
      motor[YAXIS] = 0;                   // ...the left motor is stopped with a power level of 0.
    }

    if(abs(joystick.joy1_x2) > 10) {
      motor[XAXIS] = (joystick.joy1_x2)/100.0*maxPower;
    }
    else                                      // Else if the readings are within the threshold...
    {
      motor[XAXIS] = 0;                   // ...the left motor is stopped with a power level of 0.
    }

    if(abs(joystick.joy1_y2) > 10) {
      motor[motorC] = (joystick.joy1_y2);
    }
    else                                      // Else if the readings are within the threshold...
    {
      motor[motorC] = 0;                   // ...the left motor is stopped with a power level of 0.
    }

    displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
		displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
		if (nNxtButtonPressed == 3) {
			resetAxis(XAXIS);
			resetAxis(YAXIS);
		}
	}

}
