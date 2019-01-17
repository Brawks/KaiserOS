#include "include/screen.h"
#include "include/keyb.h"
#include "include/string.h"
kmain()
{
	clearScreen();
	print("Welcome, to my castle. No really, welcome to my first kernel!\n");
	print("...\n");
	print("Sounds the same, doesn't it? I think it does.\n");
	
	while(1)
	{
		print("\n(KAISEROS) ");
		
		string ch = readStr();
		if(strEql(ch, "cmd"))
		{
			print("\nYou're already in Commando!\n");
		}
		else if(strEql(ch, "clear"))
		{
			clearScreen();
		}
		else
		{
			print("\nInvalid command. Please try again.");
		}

		print("\n");
	}
}
