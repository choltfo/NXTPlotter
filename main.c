
#include "constants.h"

#include "fileRead.c"
#include "baseFunctions.c"

void fileMenu (string & fileName) {
	// Find all .ncc files and arrange them in a string array
	TFileHandle fHandle;
	TFileIOResult IOResult;
	string fNames[32];
	short fSize = 0;


	int i = 1;
	FindFirstFile(fHandle, IOResult, "*.ncc", fNames[0], fSize);
	while (IOResult != ioRsltNoMoreFiles && i < 32){
	    fNames[i] = fNames[i-1];
		FindNextFile(fHandle, IOResult, fNames[i], fSize);
		i++;
	}
	// List all .ncc files in a menu
    eraseDisplay();
    int cSelection = 0;
    while (true) {
        eraseDisplay();
        displayCenteredBigTextLine(1,"Files: %i",cSelection);

		displayCenteredTextLine(3, "%s, %i",fNames[cSelection],i);

       	while (nNxtButtonPressed != -1) {}
       	while (nNxtButtonPressed == -1) {}

       	playSound(soundBlip);
       	eraseDisplay();
       	if (nNxtButtonPressed == 3) {
	      fileName = fNames[cSelection];
		  return;
		}

       	// Left --, right ++
       	cSelection = (cSelection + ((nNxtButtonPressed-2)*2+i+1))%i;
    }
}

int mainMenu () {
    eraseDisplay();
    int cSelection = 0;
    while (true) {
        eraseDisplay();
        displayCenteredBigTextLine(1,"Main:");

        // 0 - exit
        // 1 - file
        // 2 - joystick
        // 3 - calibrate

        switch (cSelection) {
            case 0:
            displayCenteredTextLine(3,"Exit");
            break;
            case 1:
            displayCenteredTextLine(3,"File");
            break;
            case 2:
            displayCenteredTextLine(3,"Joystick");
            break;
            case 3:
            displayCenteredTextLine(3,"Calibrate");
            break;
        }

        while (nNxtButtonPressed != -1) {}
        while (nNxtButtonPressed == -1) {}

        playSound(soundBlip);
        eraseDisplay();
        if (nNxtButtonPressed == 3) return cSelection;

        // Left --, right ++
        cSelection = (cSelection + (nNxtButtonPressed-2)*2+5)%4;
    }
}

task main() {
    int selection = -1;
    while (selection != 0) { // TODO: Use constants/enums instead of literals.

        selection = mainMenu();
        while (nNxtButtonPressed != -1){}

        if (selection == 3) {
            setTool(0);
            calibrate();
        }

        if (selection == 1) {
            /*string file = "";
            fileMenu(file);
            displayCenteredBigTextLine(1,file);
            wait10Msec(1000);*/
            readFile("MapleLeaf.ncc")
        }
        if (selection == 2) {
            TPCJoystick joystick;
            do {
                getJoystickSettings(joystick);

                if (abs(joystick.joy1_x1) > 10) {
                    motor[XAXIS] = (-joystick.joy1_y1)/100.0*maxPower;
                } else {
                    motor[XAXIS] = 0;
                }

                if (abs(joystick.joy1_x2) > 10) {
                    motor[YAXIS] = (joystick.joy1_x2)/100.0*maxPower;
                }else {
                    motor[YAXIS] = 0;
                }


                displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
                displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
                displayTextLine(3,"B: %X",joy1Btn(Btn1));
                displayTextLine(4,"Xin: %i",joystick.joy1_y2);
                displayTextLine(5,"Yin: %i",joystick.joy1_x2);

                if (nNxtButtonPressed != -1 && nNxtButtonPressed != 3) {
                    resetAxis(XAXIS);
                    resetAxis(YAXIS);
                }

            } while (nNxtButtonPressed != 3);
        }
    }
}
