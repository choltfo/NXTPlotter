
// Can't allocate in function - too big!
// Also, there's no dynamic allocation, so we can't malloc.
char incomingString[1024];

int parseInt(char * line) {
		return (line[0]>='0'&&line[0]<='9')?(line[0]-'0')
}

void parseGCode(char * text) {
	char pref = text[0];
	int opCode = parseInt(text+1);
}

void readFile(const string fileName) {
	TFileHandle file;
	TFileIOResult res = ioRsltSuccess;
	short size;

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

		parseGCode(incomingString);

		//displayTextLine(2, incomingString);
		// Do things per line right here!


		wait1Msec(1000);
	}
	// Close file
  Close(file, res);
}
