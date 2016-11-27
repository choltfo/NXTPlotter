// Final project, group 16
// Deus Ex Machina, a plot device
// November 2016
// RobotC code

// A collection of functions for reading G-code files.

#include "baseFunctions.c"

// Can't allocate inside function - too big!
// Also, there's no dynamic allocation, so we can't malloc.
char incomingString[1024];

// Charles H
// Converts an integer represented in a string to an int.
int parseInt(char*& line) {
    int sign = 1;
    int num = 0;
    if (*line == '-') {
        sign = -1;
        line++;
    }
    while (*line && *line >= '0'&& *line <= '9') {
        num = num*10 + *line - '0';
        line++;
    }
    return num*sign;
}

// Charles H
// Trims anything that's not a letter or a numer,
void trimString(char*& line) {
    while (!(
        (*line >= '0' && *line <= '9') ||
        (*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')
    )&& *line) {
        line++;
    }
}

// Charles H
// Converts a float represented in a string to a int.
float parseFloat(char*& line) {
    int sign = 1;
    float num = 0;
    if (*line == '-') {
        sign = -1;
        line++;
    }
    while (*line && *line >= '0'&& *line <= '9') {
        num = num*10 + *line - '0';
        line++;
    }

    if (*line != '.') return num*sign;
    line++;

    float prec = 1;
    while (*line && *line >= '0'&& *line <= '9') {
        prec /= 10.0;
        num = num + (*line - '0')*prec;
        line++;
    }
    return num*sign;
}

// Charles H
// Read through a G-code file, filename (xyz.ncc), and act on it.
void readFile(const string fileName) {
    TFileHandle file;
    TFileIOResult res = ioRsltSuccess;
    short size;

    time1[T1] = 0;

    // RobotC OpenRead file.
    // I did not make these functions.
    OpenRead(file, res, fileName, size);

    int line = 0;
	// Both checks if the file opens and runs through every line.
    while (res == ioRsltSuccess) {
        line++;
        char output = 0x0;
        int i = 0;
        ReadByte(file,res,output);
        incomingString[i] = output;

        // Read line.
        while (output != '\n' && res == ioRsltSuccess) {
            ++i;
            ReadByte(file,res,output);
            incomingString[i] = output;
        }

        // Perform actual G-code parsing:
        char prefix = incomingString[0];
        char * ref = incomingString;
        ref++; // You'd think this could be one line. Compiler doesn't like it.
        int opCode = parseInt(ref);
        trimString(ref);

        // Movement command.
        if (prefix == 'G') {
            // For G-ops, the coordinates are sorted X, Y
            // EG: G1 X123.34 Y126.43
            ref++; // Should be the X.

            // Get coorindates for this command.
            float comX = parseFloat(ref);
            trimString(ref);
            ref++; // Skip 'Y'
            float comY = parseFloat(ref);

            displayTextLine (1,"%s",fileName);
            displayTextLine (4,"Line: %i",line);
            if (opCode == 0) moveImmediate(comX,comY);
            if (opCode == 1) moveLinear(comX,comY);

        }

        // Tool change command.
        // Technically, should store the tool number and wait for an M6.
        // However, our path generator never puts a T# anywhere except before
        // an M6, saving us some logic on this end.
        if (prefix == 'T') {
            setTool(opCode);
        }
    }

    // Close file
    Close(file, res);

    displayCenteredBigTextLine(4,"%i s",time1[T1]/1000);
    wait1Msec(5000);
    eraseDisplay();

    // Disengage tool when done.
    setTool(0);
}
