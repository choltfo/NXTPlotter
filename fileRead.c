
// Can't allocate in function - too big!
// Also, there's no dynamic allocation, so we can't malloc.
char incomingString[1024];

// Unsafe, but all input is sanitized
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

void trimString(char*& line) {
	while (!(
		(*line >= '0' && *line <= '9') ||
		(*line >= 'a' && *line <= 'z') ||
		(*line >= 'A' && *line <= 'Z') ||
		(false)
		)) {
			line++;
	}
}

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



void readFile(const string fileName) {
	TFileHandle file;
	TFileIOResult res = ioRsltSuccess;
	short size;

	// RobotC OpenRead file.
	// "I did not make these..." functions.
  OpenRead(file, res, fileName, size);

  // Read line.
  while (res == ioRsltSuccess) {
	  char output = 0x0;
	  int i = 0;
	  ReadByte(file,res,output);
		incomingString[i] = output;

		while (output != '\n' && res == ioRsltSuccess) {
	  	++i;
			ReadByte(file,res,output);
			incomingString[i] = output;
		}

		// parseGCode(incomingString);

		char prefix = incomingString[0];
		char * ref = incomingString+1;
		int opCode = parseInt(ref);

		//displayTextLine(2, incomingString);
		// Do things per line right here!


		wait1Msec(1000);
	}
	// Close file
  Close(file, res);
}
