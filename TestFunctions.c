
#include "baseFunctions.c"

task main() {
    /*nMotorEncoder[TOOLMOTOR] = 0;
    while (true) {
        int tool = 0;
	    while (nNxtButtonPressed == -1){}

	    if (nNxtButtonPressed == 2) tool = 1;
	    if (nNxtButtonPressed == 3) tool = 0;
	    if (nNxtButtonPressed == 1) tool = 2;

        setTool(tool);
	    displayCenteredBigTextLine(2,"Tool: %i",tool);
	    while (nNxtButtonPressed != -1){}
	}*/

	SensorType[YENDSTOP] =  sensorTouch;
	SensorType[XENDSTOP] = sensorColorNxtFULL;
	while (true) {
        int tool = 0;
	    while (nNxtButtonPressed != 3){}
	    eraseDisplay();
        displayCenteredBigTextLine(2,"Cal: Y",tool);
        displayTextLine(4,"Y: %i",nMotorEncoder[YAXIS]);
        calibrateAxis(false);
        eraseDisplay();
        displayCenteredBigTextLine(2,"Cal: Y",tool);
	    displayTextLine(4,"Y: %i",nMotorEncoder[YAXIS]);
	    while (nNxtButtonPressed != -1){}

	}
}
