#pragma once
#include <string>

//Byte containers
static char wholeFile[0x17B];
static char cpuDifficulty[1];
static char winCountArcade[1];
static char damageLevel[1];
static char timerSpeed[1];
static char winCountVersus[1];
static char saveReplay[1];
static char bgmVolume[1];
static char sfxVolume[1];
static char characterFilter[1];
static char stageAnimations[1];
static char viewFps[1];
static char frameRate[1];
static char screenFilter[1];
static char aspectRatio[1];

void clearScreen(void); /* Clears the screen */
/* The implementation varies depending on the OS, the behavior is defined in Program.cpp*/

#ifdef __unix__
char _getch(void); // Pause the execution and wait for an unbuffered keyboard input, then return the character and resume
#endif

#ifdef _WIN32
#include <conio.h>
#include <tchar.h>
#endif //win32

class Program
{
public:
	class Invalid { public: std::string what{ "" }; };
};

