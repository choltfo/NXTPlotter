
#include "constants.h"

#include "fileRead.c"
#include "baseFunctions.c"


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
        if (selection == 1) {
            readFile("mapleLeaf.ncc");
        }
        if (selection == 2) {
            TPCJoystick joystick;
            do {
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

                if (joystick.joy1_x2 > 100) {
                    nMotorEncoder[TOOLMOTOR]=0;
                    while(nMotorEncoder[TOOLMOTOR]<90) {
                        motor[TOOLMOTOR]=50;
                    }
                    motor[TOOLMOTOR]=0;
                    nMotorEncoder[TOOLMOTOR]=0;
                }

                displayTextLine(1,"X: %f",getCurrentAxis(XAXIS));
                displayTextLine(2,"Y: %f",getCurrentAxis(YAXIS));
                displayTextLine(3,"B: %X",joy1Btn(Btn1));
                displayTextLine(4,"Xin: %i",joystick.joy1_x1);
                displayTextLine(5,"Yin: %i",joystick.joy1_x2);

                if (nNxtButtonPressed == 3) {
                    resetAxis(XAXIS);
                    resetAxis(YAXIS);
                }

            } while (!joy1Btn(1));
        }
    }
}
