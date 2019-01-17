#include "screen.h"

int cursorX = 0, cursorY = 0;			//I guess we define intengers for our Mouse Values
const uint8 sw = 80, sh = 25, sd = 2;		//We define constants for the screen width, height and depth
void clearLine(uint8 from,uint8 to)		//This void that we create is what permits us to delete lines of text on the screen, when called, use parameters to define from which section to which
{
	uint16 i = sw * from * sd;
	string vidmem=(string)0xb8000;
	for(i; i<(sw*(to+1)*sd); i++) 
	{
		vidmem[i] = 0x0;		//For each character the string contains from the section "from" to "to", its color will become black until i reaches the maximum number of characters.
	}
}

void updateCursor()				//Updates the cursor. Didn't expect that, did you?
{
	unsigned temp;
	
	temp = cursorY * sw + cursorX;		//Position = (y * screenwidth) + x

	outportb(0x3D4, 14);			//CRT Control Register : Select Cursor Location
	outportb(0x3D5, temp >> 8);		//Send the high byte across the bus
	outportb(0x3D4, 15);			//CRT Control Register : Select Send Low byte
	outportb(0x3D5, temp);			//Send the low byte of the cursor location
}

void clearScreen()				//Clears the screen from line 0 to the screensize minus 1 line. Of course.
{
	clearLine(0,sh-1);
	cursorX = 0;
	cursorY = 0;
	updateCursor();
}

void scrollUp(uint8 lineNumber)
{
	string vidmem = (string)0xb8000;
	uint16 i = 0;
	for (i; i<sw*(sh-1)*sd; i++)
	{
		vidmem[i] = vidmem[i+sw*2*lineNumber];
	}
	clearLine(sh-1-lineNumber,sh-1);
	if((cursorY - lineNumber) < 0)
	{
		cursorY = 0;
		cursorX = 0;	
	}
	else
	{
		cursorY -= lineNumber;
	}
	updateCursor();
}

void newLineCheck()				//Scroll up 1 line if if the text passes the screen size
{
	if(cursorY>sh-1)
	{
		scrollUp(1);
	}
}

void printch(char c)
{
	string vidmem = (string)0xb8000;
	switch(c)
	{
		case (0x08) :			//Backspace to delete
			if (cursorX > 0)
			{
				cursorX --;
				vidmem[(cursorY * sw + cursorX)*sd]=0x00; //Color of the last char becomes black (deleted)
			}
			break;
		case (0x09) :			//Tab button
			cursorX = (cursorX + 8) & ~(8 - 1);
			break;
		case ('\r') : 			//Start the text from the beginning
			cursorX = 0;
			break;
		case ('\n') :			//Jumps to the next line
			cursorX = 0;
			cursorY++;
			break;
		default :
			vidmem[((cursorY * sw + cursorX)*sd)] = c; 		//Prints Character 
			vidmem[((cursorY * sw + cursorX)*sd+1)] = 0x02;		//Color - in this case its white background (0) on green foreground (2)
			cursorX++;
			break;
	}
	if(cursorX >= sw)			//Jumps line when text (x cursor) reaches the end of the screen
	{
		cursorX = 0;
		cursorY++;
	}
	newLineCheck();
	updateCursor();
}

void print(string ch)				//Function that loops the printch function for everycharacter within the string parameter, so the text can appear on the screen (switch case checked)
{
	uint16 i = 0;
	uint8 length = strlength(ch)-1;		//Storing length to a variable to prevent calling the function twice when comparing strings
	for(i; i<length;i++)
	{
		printch(ch[i]);
	}
}
