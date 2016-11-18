
#include "constants.h"

#include "fileRead.c"
#include "baseFunctions.c"


task main() {

	resetAxis(XAXIS);
	resetAxis(YAXIS);

	/*moveLinear(100,100);
	wait1Msec(1000);
	moveLinear(-100,100);
	wait1Msec(1000);
	moveLinear(-100,-100);
	wait1Msec(1000);*/

	setTool(2);
	setTool(1);

	wait10Msec(1000);
	readFile("MapleLeaf.ncc");

/*
	TPCJoystick joystick;
	while (true) {
		getJoystickSettings(joystick);

		if (abs(joystick.joy1_y1) > 10) {
      motor[YAXIS] = (joystick.joy1_y1)/100.0*maxPower;
    } else {
      motor[YAXIS] = 0;                   // ...the left motor is stopped with a power level of 0.
    }

    if (abs(joystick.joy1_x2) > 10) {
      motor[XAXIS] = (joystick.joy1_x2)/100.0*maxPower;
    }else {
      motor[XAXIS] = 0;                   // ...the left motor is stopped with a power level of 0.
    }

    if (abs(joystick.joy1_y2) > 10) {
      motor[motorC] = (joystick.joy1_y2);
    } else {
			motor[motorC] = 0;
		}

		displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
		displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));

		if (nNxtButtonPressed == 3) {
			resetAxis(XAXIS);
			resetAxis(YAXIS);
		}
	}
*/
}
