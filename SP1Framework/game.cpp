// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include "mALL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

double  ElapsedTime;
double  DeltaTime;
bool    KeyPressed[K_COUNT];

//setting of maze
bool activatetrap1 = false;  //for maze1
bool activatetrap2 = false;  //for maze2
bool activatetrap3 = false;  //for maze2

bool eventactivate1 = false; //for maze1
bool eventactivate2 = false; //for maze2
bool eventactivate3 = false; //for maze3

bool opentrap1 = false;      //for maze1
bool opentrap2 = false;
bool opentrap3 = false;      //for maze2

bool opentrap2z = false;      //for maze3
bool opentrap2o = false;      //for maze3
bool opentrap2k = false;      //for maze3
bool opentrap2y = false;      //for maze3
bool opentrap2v = false;      //for maze3


bool clearevent1 = false;
bool clearevent2 = false;
bool clearevent3 = false;

int mazelevel = 0;

void renderMapOpenTrap1();
void renderMapOpenTrap2();
void renderMapOpenTrap3();

void renderMapEventClear1();
void renderMapEventClear2();
void renderMapEventClear3();
bool playmusic = false;
void rendermainmenu();
bool opendoor = false;

// Game specific variables here
SGameChar   Char;

EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
// Console object
Console console(70, 21, "SP1 Framework");
//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{

	//g_dBounceTime = ElapsedTime + 0.125;
	if (playmusic == false)
	{
		// Set precision for floating point output
		ElapsedTime = 0.0;
		g_dBounceTime = 0.0;

		// sets the initial state for the game
		g_eGameState = S_SPLASHSCREEN;

		Char.character.X = 0;
		Char.character.Y = 2;

		Char.m_bActive = true;
		// sets the width, height and the font name to use in the console
		console.setConsoleFont(0, 16, L"Consolas");
		playmusic = true;
		init();
	}
	else
	{
		MusicInit();
		if (mazelevel == 0)
		{
			MusicPlay("tacos", "repeat");
		}
		else if (mazelevel == 1)
		{
			MusicStop("tacos");
			MusicPlay("mazelevel1", "repeat");
		}
		else if (mazelevel == 2)
		{
			MusicStop("mazelevel1");
			MusicPlay("mazelevel2", "repeat");
		}
		else if (mazelevel == 3)
		{
			MusicStop("mazelevel2");
			MusicPlay("mazelevel3", "repeat");
		}
	}
	Char.pointer.X = 21;
	Char.pointer.Y = 10;
	Char.mainmenu = false;
}
//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    console.clearBuffer();
}
//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void keyboard( void )
{    
	KeyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
    KeyPressed[K_UP]     = isKeyPressed(VK_UP);
    KeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    KeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    KeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    KeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    KeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	KeyPressed[K_JUMP] = isKeyPressed(VK_KEY_J);
}
//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    ElapsedTime += dt;
    DeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void loadscreen()
{
	
    clearScreen();      // clears the current screen and draw from scratch 
	switch (g_eGameState)
	{
	case S_SPLASHSCREEN: renderSplashScreen();
		break;
	case S_GAME: renderGame();
            break;
    }

    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
	g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
	
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	                    // sound can be played here too.
}

char scanMap1(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE1.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap1E(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE1E.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				if (opentrap1 == true)
				{
					if (line[j] == 'x' || line[j] == 'X')
					{
						line[j] = ' ';
					}
				}
				
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap1D(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE1D.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}

char scanMap2(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE2.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				if (opentrap2z == true)
				{
					if (line[j] == 'z')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2o == true)
				{
					if (line[j] == 'o')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2k == true)
				{
					if (line[j] == 'k')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2v == true)
				{
					if (line[j] == 'v')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2y == true)
				{
					if (line[j] == 'y')
					{
						line[j] = ' ';
					}
				}
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap2E(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE2E.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
					if (opendoor == true)
					{
						if (line[j] == 'D')
						{
							line[j] = ' ';
						}
					}
					if (opentrap2 == true && opendoor == true)
					{
						if (line[j] == 'V')
						{
							line[j] = ' ';
						}
					}
				
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap2D(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE2D.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				if (opentrap2z == true)
				{
					if (line[j] == 'z')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2o == true)
				{
					if (line[j] == 'o')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2k == true)
				{
					if (line[j] == 'k')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2v == true)
				{
					if (line[j] == 'v')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2y == true)
				{
					if (line[j] == 'y')
					{
						line[j] = ' ';
					}
				}
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}


char scanMap3(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE3.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap3E(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE3E.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}
char scanMap3D(char wall[20][70])
{
	string line;
	ifstream myfile("MAZE3D.txt");
	int i = 0;
	int pos = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	return wall[20][70];
}



// character movement and event trigger
void moveCharacter()
{

	bool somethinghappened = false;
	if (g_dBounceTime > ElapsedTime)
		return;
	char wall[20][70];
	// maze level 1
	if (mazelevel == 0)
	{
		// chosing 1E or 1D
		if (eventactivate1 == true)
		{
			// after finishing 1e
			if (clearevent1 == true)
			{
				scanMap1D(wall);
			}
			//moving to 1E
			else
			{
				scanMap1E(wall);
			}
		}
		//moving to maze 1
		else
		{
			scanMap1(wall);
		}
	}
	//maze level 2
	else if (mazelevel == 1)
	{
		//chosing maze 2E or 2D
		if (eventactivate2 == true)
		{
			//after finishing 2E
			if (clearevent2 == true)
			{
				scanMap2D(wall);
			}
			//moving to 2E
			else
			{
				scanMap2E(wall);
			}
		}
		// moving to maze 2
		else
		{
			scanMap2(wall);
		}
	}
	//maze level 3
	else if (mazelevel == 2)
	{
		//chosing of maze 3E or 3D
		if (eventactivate3 == true)
		{
			// after finishing maze 3E
			if (clearevent3 == true)
			{
				scanMap3D(wall);
			}
			// going to maze 3 E
			else
			{
				scanMap3E(wall);
			}
		}
		//going to maze 3
		else
		{
			scanMap3(wall);
		}
	}


	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the characterj

	// maze 2 e special skills "blink" 	// J + direction 
	if (mazelevel == 1 && eventactivate2==true && clearevent2==false)
	{
		if (KeyPressed[K_JUMP] && KeyPressed[K_RIGHT] && Char.character.X < 68)
		{
			int characterlocationY = Char.character.Y - 1;
			int characterlocationX = Char.character.X;
			characterlocationX += 1;
			if (wall[characterlocationY][characterlocationX] == '#')
			{
				characterlocationX += 1;
				if (wall[characterlocationY][characterlocationX] != '#')
				{
					Char.character.X++;
				}
			}
		}
		if (KeyPressed[K_JUMP] && KeyPressed[K_DOWN] && Char.character.Y < 18)
		{
			int characterlocationY = Char.character.Y - 1;
			int characterlocationX = Char.character.X;
			characterlocationY += 1;
			if (wall[characterlocationY][characterlocationX] == '#')
			{
				characterlocationY += 1;
				if (wall[characterlocationY][characterlocationX] != '#')
				{
					Char.character.Y++;
				}
			}
		}
		if (KeyPressed[K_JUMP] && KeyPressed[K_UP] && Char.character.Y > 2)
		{
			int characterlocationY = Char.character.Y - 1;
			int characterlocationX = Char.character.X;
			characterlocationY -= 1;
			if (wall[characterlocationY][characterlocationX] == '#')
			{
				characterlocationY -= 1;
				if (wall[characterlocationY][characterlocationX] != '#')
				{
					Char.character.Y--;
				}
			}
		}
		if (KeyPressed[K_JUMP] && KeyPressed[K_LEFT] && Char.character.X > 2)
		{
			int characterlocationY = Char.character.Y - 1;
			int characterlocationX = Char.character.X;
			characterlocationX -= 1;
			if (wall[characterlocationY][characterlocationX] == '#')
			{
				characterlocationX -= 1;
				if (wall[characterlocationY][characterlocationX] != '#')
				{
					Char.character.X--;
				}
			}
		}
	}


	if (KeyPressed[K_UP] && Char.character.Y > 2)
	{
		Beep(1440, 30);
		int characterlocationY = Char.character.Y - 1;
		int characterlocationX = Char.character.X;
		characterlocationY -= 1;
		//for maze 1
		if (mazelevel == 0)
		{
		
		    if (wall[characterlocationY][characterlocationX] == ' '|| wall[characterlocationY][characterlocationX] == 'V' || 
				wall[characterlocationY][characterlocationX] == 's' || wall[characterlocationY][characterlocationX] == 'S' )
			{

				Char.character.Y--;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 's')
				{
					Char.character.X = 1;
					Char.character.Y = 14;
				}
				if (wall[characterlocationY][characterlocationX] == 'V')
				{
					opentrap1 = false;
					renderGame();
				}

			}

		}
		//for maze 2
		if (mazelevel == 1)
		{
			if (wall[characterlocationY][characterlocationX] == ' '|| wall[characterlocationY][characterlocationX] == 'E'|| 
				wall[characterlocationY][characterlocationX] == 'V'|| wall[characterlocationY][characterlocationX] == 'O'|| 
				wall[characterlocationY][characterlocationX] == 'X'|| wall[characterlocationY][characterlocationX] == 'C'|| 
				wall[characterlocationY][characterlocationX] == 'R'|| wall[characterlocationY][characterlocationX] == 'B')
			{
				Char.character.Y--;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 'E')
				{
					Char.character.X = 0;
					Char.character.Y = 2;
					eventactivate2 = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'V')
				{
					opentrap2v = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'O')
				{
					opentrap2o = true;
					renderGame();
				}
				if (eventactivate2 == true)
				{
					if (wall[characterlocationY][characterlocationX] == 'X')
					{
						opentrap2 = true;
						renderGame();
					}
				}
				if (wall[characterlocationY][characterlocationX] == 'C')
				{
					opendoor = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'R')
				{
					Char.character.X = 57;
					Char.character.Y = 3;
				}

			}
		}
	}
		
	if (KeyPressed[K_LEFT] && Char.character.X > 0)
	{
		Beep(1440, 30);

		int characterlocationY = Char.character.Y - 1;
		int characterlocationX = Char.character.X;
		characterlocationX -= 1;
		//for maze 1
		if (mazelevel == 0)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == 'E' ||
				wall[characterlocationY][characterlocationX] == 'V' || wall[characterlocationY][characterlocationX] == 's')
			{
				Char.character.X--;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 'E')
				{
					Char.character.X = 0;
					Char.character.Y = 2;
					eventactivate1 = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 's')
				{
					Char.character.X = 1;
					Char.character.Y = 14;
				}
				if (wall[characterlocationY][characterlocationX] == 'V')
				{
					opentrap1 = false;
					renderGame();
				}

			}
		}
		//for maze 2
		if (mazelevel == 1)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == 'V' ||
				wall[characterlocationY][characterlocationX] == 'Y' || wall[characterlocationY][characterlocationX] == 'K' ||
				wall[characterlocationY][characterlocationX] == 'C' || wall[characterlocationY][characterlocationX] == 'R' ||
				wall[characterlocationY][characterlocationX] == 'B')
			{
				Char.character.X--;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 'R')
				{
					Char.character.X = 57;
					Char.character.Y = 3;
				}
				if (wall[characterlocationY][characterlocationX] == 'C')
				{
					opendoor = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'B')
				{
					opendoor = false;
					renderGame();
				}
			}

		}


	}

	if (KeyPressed[K_DOWN] && Char.character.Y < 19)
	{
		Beep(1440, 30);
		int characterlocationY = Char.character.Y - 1;
		int characterlocationX = Char.character.X;
		characterlocationY += 1;
		//for maze 1
		if (mazelevel == 0)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == 'X' ||
				wall[characterlocationY][characterlocationX] == 'S')
			{
				Char.character.Y++;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 'X')
				{
					opentrap1 = true;
					renderGame();
				}
			}
		}
		//for maze 2
		if (mazelevel == 1)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == 'C' ||
				wall[characterlocationY][characterlocationX] == 'R' || wall[characterlocationY][characterlocationX] == 'B' ||
				wall[characterlocationY][characterlocationX] == 'X')
			{
				Char.character.Y++;
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == 'C')
				{
					opendoor = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'B')
				{
					opendoor = false;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'R')
				{
					Char.character.X = 57;
					Char.character.Y = 3;
				}
				if (wall[characterlocationY][characterlocationX] == 'X')
				{
					opentrap2 = true;
					renderGame();
				}
			}
		}

	}

	if (KeyPressed[K_RIGHT] && Char.character.X < 69)
	{
		//PlaySound(TEXT("Wii Music - Gaming Background Music (HD).wav"), NULL, SND_FILENAME | SND_ASYNC); //search for press and hold keys
		Beep(1440, 30);
		int characterlocationY = Char.character.Y - 1;
		int characterlocationX = Char.character.X;
		characterlocationX += 1;




		//for maze1
		if (mazelevel == 0)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == '2' ||
				wall[characterlocationY][characterlocationX] == 'E' || wall[characterlocationY][characterlocationX] == 's' ||
				wall[characterlocationY][characterlocationX] == 'T')
			{

				Char.character.X++;
				somethinghappened = true;
				//going to maze 2
				if (wall[characterlocationY][characterlocationX] == '2')
				{
					Char.character.X = 0;
					Char.character.Y = 2;
					mazelevel++;
					renderGame();
				}
				// going to event maze
				if (wall[characterlocationY][characterlocationX] == 'E')
				{
					Char.character.X = 0;
					Char.character.Y = 2;
					eventactivate1 = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 's')
				{
					Char.character.X = 1;
					Char.character.Y = 14;
				}
				if (Char.character.Y == 13 && Char.character.X == 69)
				{
					Char.character.X = 33;
					Char.character.Y = 2;

					clearevent1 = true;

					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'T')
				{
					Char.character.X = 60;
					Char.character.Y = 12;
				}
			}
		}
		//for maze2 
		if (mazelevel == 1)
		{
			if (wall[characterlocationY][characterlocationX] == ' ' || wall[characterlocationY][characterlocationX] == 'O' ||
				wall[characterlocationY][characterlocationX] == 'K' || wall[characterlocationY][characterlocationX] == 'Y' ||
				wall[characterlocationY][characterlocationX] == 'Z' || wall[characterlocationY][characterlocationX] == '3' ||
				wall[characterlocationY][characterlocationX] == 'C' || wall[characterlocationY][characterlocationX] == 'R' ||
				wall[characterlocationY][characterlocationX] == 'B')
			{
				Char.character.X++;
				
				//if (KeyPressed[K_RIGHT] && Char.character.X < 69)
				//{
				//	somethinghappened1 = true;
				//	MusicInit();
				//	MusicPlay("alias", "repeat");
				//	MusicStop("alias");
				//	MusicPlay("alias", " ");
				//	//PlaySound(TEXT("Wii Music - Gaming Background Music (HD).wav"), NULL, SND_FILENAME | SND_ASYNC);
				//}
				somethinghappened = true;
				if (wall[characterlocationY][characterlocationX] == '3')
				{
					Char.character.X = 0;
					Char.character.Y = 2;
					mazelevel++;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'O')
				{
					opentrap2o = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'Y')
				{
					opentrap2y = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'V')
				{
					opentrap2v = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'K')
				{
					opentrap2k = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'Z')
				{
					opentrap2z = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'C')
				{
					opendoor = true;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'B')
				{
					opendoor = false;
					renderGame();
				}
				if (wall[characterlocationY][characterlocationX] == 'R')
				{
					Char.character.X = 57;
					Char.character.Y = 3;
				}
				if (Char.character.Y == 2 && Char.character.X == 69)
				{
					Char.character.X = 7;
					Char.character.Y = 16;
					clearevent2 = true;
					renderGame();
				}
			}
		}
	}

	if (KeyPressed[K_SPACE])
	{
		Char.m_bActive = !Char.m_bActive;
		somethinghappened = true;
	}

	if (somethinghappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = ElapsedTime + 0.125; // 125ms should be enough
	}
}


//loading of 1D
void renderMapEventClear1() 
{
	COORD c;
	char wall[20][70];
	string line;
	int i = 0;
	int pos = 0;
	if (mazelevel == 0)
	{
		if (eventactivate1 == true)
		{
			ifstream myfile("MAZE1D.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if (line[j] == '2'||line[j]=='k')
						{
							line[j] = ' ';
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
	}
	for (int k = 0; k < 20; k++)
	{
		int position = 0;
		c.Y = 1 + pos;
		for (int j = 0; j < 70; j++)
		{
			c.X = position;
			if (wall[k][j] == ' ')
			{
				console.writeToBuffer(c, wall[k][j], 0x0);
			}
			else
			{
				console.writeToBuffer(c, wall[k][j], 0xFFF);
			}
			position++;
		}
		pos++;
	}
}
//loading of 2D
void renderMapEventClear2()
{
	COORD c;
	char wall[20][70];
	string line;
	int i = 0;
	int pos = 0;

	ifstream myfile("MAZE2D.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				if (opentrap2z == true)
				{
					if (line[j] == 'z')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2o == true)
				{
					if (line[j] == 'o')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2k == true)
				{
					if (line[j] == 'k')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2v == true)
				{
					if (line[j] == 'v')
					{
						line[j] = ' ';
					}
				}
				if (opentrap2y == true)
				{
					if (line[j] == 'y')
					{
						line[j] = ' ';
					}
				}
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}
	for (int k = 0; k < 20; k++)
	{
		int position = 0;
		c.Y = 1 + pos;
		for (int j = 0; j < 70; j++)
		{
			c.X = position;
			if (wall[k][j] == ' ' || wall[k][j] == '2' || wall[k][j] == 'k' ||
				wall[k][j] == 'E' || wall[k][j] == 'S' || wall[k][j] == 's' ||
				wall[k][j] == 'X' || wall[k][j] == 'T' || wall[k][j] == '3' ||
				wall[k][j] == 'Y' || wall[k][j] == 'V' || wall[k][j] == 'A' ||
				wall[k][j] == 'O' || wall[k][j] == 'C' || wall[k][j] == 'R' ||
				wall[k][j] == 'B')
			{
				console.writeToBuffer(c, wall[k][j], 0x0);
			}
			else
			{
				console.writeToBuffer(c, wall[k][j], 0xFFF);
			}
			position++;
		}
		pos++;
	}
}




// opening event1 trap
void renderMapOpenTrap1()
{
	COORD c;
	char wall[20][70];
	string line;
	int i = 0;
	int pos = 0;
	ifstream myfile("MAZE1E.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int j = 0; j < 70; j++)
			{
				if (line[j] == 'x'||line[j]=='V')
				{
					line[j] = ' ';
				}
				
				wall[i][j] = line[j];
			}
			i++;
		}
		myfile.close();
	}

	for (int k = 0; k < 20; k++)
	{
		int position = 0;
		c.Y = 1 + pos;
		for (int j = 0; j < 70; j++)
		{
			c.X = position;
			if (wall[k][j] == ' ' || wall[k][j] == '2' || wall[k][j] == 'k' ||
				wall[k][j] == 'E' || wall[k][j] == 'S' || wall[k][j] == 's' ||
				wall[k][j] == 'X' || wall[k][j] == 'T')
			{
				console.writeToBuffer(c, wall[k][j], 0x0);
			}
			else
			{
				console.writeToBuffer(c, wall[k][j], 0xFFF);
			}
			position++;
		}
		pos++;
	}

}
// opening event 2 trap
void renderMapOpenTrap2()
{
	COORD c;
	char wall[20][70];
	string line;
	int i = 0;
	int pos = 0;
	if (mazelevel == 1)
	{
		if (eventactivate2 == true)
		{
			ifstream myfile("MAZE2E.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if (line[j] == 'X' || line[j] == 'C' || line[j] == 'B' || line[j] == 'R')
						{
							line[j] = ' ';
						}
						if (opentrap2 == true && opendoor==true)
						{
							if (line[j] == 'V')
							{
								line[j] = ' ';
							}
						}
						if (opendoor == true)
						{
							if (line[j] == 'D')
							{
								line[j] = ' ';
							}
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
		for (int k = 0; k < 20; k++)
		{
			int position = 0;
			c.Y = 1 + pos;
			for (int j = 0; j < 70; j++)
			{
				c.X = position;
				if (wall[k][j] == ' ' )
				{
					console.writeToBuffer(c, wall[k][j], 0x0);
				}
				else if ((wall[k][j] == 'V') && (eventactivate2 == true))
				{
					wall[k][j] = ' ';
					console.writeToBuffer(c, wall[k][j], 0x13);
				}
				else
				{
					console.writeToBuffer(c, wall[k][j], 0xFFF);
				}
				position++;
			}
			pos++;
		}
	}
}


// loading setting

//loading of character location
void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (Char.m_bActive)
    {
        charColor = 0x0A;
    }
    console.writeToBuffer(Char.character, (char)1, charColor);
}
//loading of frame
void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / DeltaTime << "fps";
    c.X = console.getConsoleSize().X - 9;
    c.Y = 0;
    console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << ElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    console.writeToBuffer(c, ss.str(), 0x59);
}
//loading to coord
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    console.flushBufferToConsole();
}
//cls
void clearScreen()
{
	// Clears the buffer with this colour attribute
	console.clearBuffer(0x00);
}
// renders the starting  screen
void renderSplashScreen()
{
	renderGame();
}


void rendermainmenu()
{
	COORD c;
	c.Y = 4;
	c.X = 27;
	console.writeToBuffer(c, "A-maze-ing Game", 0x03);
	c.Y = 7;
	c.X = 18;
	console.writeToBuffer(c, "Please  Select  Your  Game  Mode", 0x07);
	c.Y = 10;
	c.X = 28;
	console.writeToBuffer(c, "Story Mode", 0x05);
	c.Y = 13;
	c.X = 27;
	console.writeToBuffer(c, "Special Mode", 0x05);
	c.Y = 16;
	c.X = 28;
	console.writeToBuffer(c, "Exit Game", 0x05);
	console.writeToBuffer(Char.pointer, "->", 0x06);
	bool pointerchanged = false;
	if (g_dBounceTime > ElapsedTime)
		return;
	if (KeyPressed[K_DOWN] && Char.pointer.Y < 16)
	{
		Char.pointer.Y += 3;
		pointerchanged = true;
	}
	if (KeyPressed[K_UP] && Char.pointer.Y > 10)
	{
		Char.pointer.Y -= 3;
		pointerchanged = true;
	}
	if (pointerchanged == true)
	{
		g_dBounceTime = ElapsedTime + 0.125;
	}
	if (KeyPressed[K_RETURN] && Char.pointer.Y == 10)
	{
	}
	if (KeyPressed[K_RETURN] && Char.pointer.Y == 13)
	{
		Char.mainmenu = true;
		renderGame();
	}
	if (KeyPressed[K_RETURN] && Char.pointer.Y == 16)
	{
		escgame = true;
	}
 }
//loading of event / D map
void renderGame()
{
	//open maze1E trap

	if (Char.mainmenu == false)
	{
		rendermainmenu();
	}
	else
	{
		if (opentrap1 == true && clearevent1 == false && mazelevel == 0)
		{
			renderMapOpenTrap1();
		}
		//maze 1D 
		else if (clearevent1 == true && opentrap1 == true && mazelevel == 0)
		{
			renderMapEventClear1();
		}
		//maze 2 

		else if ((opentrap2 == true || opendoor == true) && mazelevel == 1)
		{
			//maze2D 
			if (clearevent2 == true)
			{
				renderMapEventClear2();
			}
			//open maze2E trap
			else
			{
				renderMapOpenTrap2();
			}
		}
		//open normal / event map
		else
		{
			renderMap();
		}
		//update character location

		renderCharacter();
	}
}
//loading map
void renderMap()
{
	COORD c;
	char wall[20][70];
	string line;
	int i = 0;
	int pos = 0;
	//loading maze 1 or 1E
	if (mazelevel == 0)
	{

		//loading maze 1E
		if (eventactivate1 == true)
		{
			ifstream myfile("MAZE1E.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if (line[j] == 'V' || line[j] == 'X' || line[j] == 's' || line[j] == 'S' || line[j] == 'T')
						{
							line[j] = ' ';
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
		//loading maze 1
		else
		{
			ifstream myfile("MAZE1.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if ( line[j] == '2' || line[j] == 'E' || line[j] == 'k')
						{
							line[j] = ' ';
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
	}
	//loading maze 2 or 2E
	else if (mazelevel == 1)
	{
		//loading maze 2E
		if (eventactivate2 == true)
		{
			ifstream myfile("MAZE2E.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if (line[j] == 'X' || line[j] == 'C' || line[j] == 'B'|| line[j] == 'R')
						{
							line[j] = ' ';
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
		//loading maze 2
		else
		{
			ifstream myfile("MAZE2.txt");
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						if (opentrap2z == true)
						{
							if (line[j] == 'z')
							{
								line[j] = ' ';
							}
						}
						if (opentrap2o == true)
						{
							if (line[j] == 'o')
							{
								line[j] = ' ';
							}
						}
						if (opentrap2k == true)
						{
							if (line[j] == 'k')
							{
								line[j] = ' ';
							}
						}
						if (opentrap2v == true)
						{
							if (line[j] == 'v')
							{
								line[j] = ' ';
							}
						}
						if (opentrap2y == true)
						{
							if (line[j] == 'y')
							{
								line[j] = ' ';
							}
						}
						if (line[j] == 'O'||line[j]=='K'||line[j]=='Z'||line[j]=='Y'||line[j]=='V'||line[j]=='E'||line[j]=='F'||line[j]=='3')
						{
							line[j] = ' ';
						}
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
	}
	//loading maze 3 or 3E
	else if (mazelevel == 2)
	{
		//loading maze 3E
		if (eventactivate3 == true)
		{
			ifstream myfile("MAZE3E.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}

		}
		//loading maze 3
		else
		{
			ifstream myfile("MAZE3.txt");

			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					for (int j = 0; j < 70; j++)
					{
						wall[i][j] = line[j];
					}
					i++;
				}
				myfile.close();
			}
		}
	}
	// wall and path drawing
	for (int k = 0; k < 20; k++)
	{
		int position = 0;
		c.Y = 1 + pos;
		for (int j = 0; j < 70; j++)
		{
			c.X = position;
			// for black
			if (wall[k][j] == ' ' )
			{
				console.writeToBuffer(c, wall[k][j], 0x0); 
			}
			//for maze2 E ending door color
			else if ((wall[k][j] == 'V')&&(eventactivate2 == true)&&(mazelevel==1)&&(clearevent2==false))
			{
				wall[k][j] = ' ';
				console.writeToBuffer(c, wall[k][j], 0x13);
			}
			else
			{
				console.writeToBuffer(c, wall[k][j], 0xFFF);
			}
			position++;
		}
		pos++;
	}
}
//esc button
void processUserInput()
{
	// quits the game if player hits the escape key
	if (KeyPressed[K_ESCAPE])
		escgame = true;
}