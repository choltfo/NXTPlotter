// Final project, group 16
// Deus Ex Machina, a plot device
// November 2016
// RobotC code

// Contains the main task, and user interface functions.

#include "constants.h"

#include "fileRead.c"
#include "baseFunctions.c"

// Matthew G
bool fileMenu (string & fileName) {
    nNxtExitClicks = 2;

    // Find all .ncc files and arrange them in a string array
    TFileHandle fHandle;
    TFileIOResult IOResult;
    string fNames[10];
    short fSize = 0;


    int i = 1;
    FindFirstFile(fHandle, IOResult, "*.ncc", fNames[0], fSize);
    while (IOResult == ioRsltSuccess && i < 10){
        fNames[i] = fNames[i-1];
        FindNextFile(fHandle, IOResult, fNames[i], fSize);
        i++;
    }
    // List all .ncc files in a menu
    eraseDisplay();
    int cSelection = 0;
	
	// Returns from within this loop - it's not infinite.
    while (true) {
        eraseDisplay();
        displayCenteredBigTextLine(1,"Files: %i",i);

        displayCenteredTextLine(3, "%s",fNames[cSelection]);

        while (nNxtButtonPressed != -1) {}
        while (nNxtButtonPressed == -1) {}

        playSound(soundBlip);
        eraseDisplay();
        if (nNxtButtonPressed == 3) {
            fileName = fNames[cSelection];
            return true;
        }
        if (nNxtButtonPressed == 0) {
            return false;
        }

        // Left --, right ++
        cSelection = (cSelection + ((nNxtButtonPressed-2)*2+i+1))%i;
    }
}

// Charles H
// The main menu.
// Returns the index of the menu item selected.
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

        // Left +, right --
        cSelection = (cSelection + (nNxtButtonPressed-2)*2+5)%4;
    }
}

// Namoos Y
// Sets the motor and tooling according to joystick input.
// The axis names make no sense, but that's an artifact of
//  RobotC.
void joystickControl () {
    TPCJoystick joystick;
    do {
        getJoystickSettings(joystick);

        if (abs(joystick.joy1_x1) > 10) {
            motor[XAXIS] = (-joystick.joy1_y1)/100.0*maxPower;
            } else {
            motor[XAXIS] = 0;
        }

        if (abs(joystick.joy1_x2) > 10) {
            motor[YAXIS] = (joystick.joy1_x2)/-100.0*maxPower;
            }else {
            motor[YAXIS] = 0;
        }

        if (abs(joystick.joy1_y2) > 100) {
            if (joystick.joy1_y2 > 0) {
                if (abs(getTool()) == 2)
                    setTool(0);
                else if (getTool() == 0 || getTool() == -1)
                    setTool(1);
            } else {
                if (abs(getTool()) == 1)
                    setTool(0);
                else if (getTool() == 0 || getTool() == -2)
                    setTool(2);
            }
            while(abs(joystick.joy1_y2) > 10) {
                getJoystickSettings(joystick);
            }
        }




        displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
        displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
        displayTextLine(3,"T: %i",getTool());
        displayTextLine(4,"y1: %i",joystick.joy1_y1);
        displayTextLine(5,"y2: %i",joystick.joy1_y2);

        if (nNxtButtonPressed != -1 && nNxtButtonPressed != 3) {
            resetAxis(XAXIS);
            resetAxis(YAXIS);
        }

    } while (nNxtButtonPressed != 3);
}

// Charles H
// Takes the number from the main menu and launches the corresponding function.
task main() {
    int selection = -1;
    while (selection != 0) {

        selection = mainMenu();
        while (nNxtButtonPressed != -1){}

        if (selection == 3) {
            setTool(0);
            calibrate();
        }

        if (selection == 1) {
            string file = "";
            if (fileMenu(file)) {
                readFile(file);
                playSound(soundBeepBeep);
            }
        }
        if (selection == 2) {
            joystickControl ();
        }
    }

}
