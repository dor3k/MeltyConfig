#pragma once
#include <string>

//Byte containers
static unsigned char wholeFile[0x17B];
static unsigned char cpuDifficulty[1];
static unsigned char winCountArcade[1];
static unsigned char damageLevel[1];
static unsigned char timerSpeed[1];
static unsigned char winCountVersus[1];
static unsigned char saveReplay[1];
static unsigned char bgmVolume[1];
static unsigned char sfxVolume[1];
static unsigned char characterFilter[1];
static unsigned char stageAnimations[1];
static unsigned char viewFps[1];
static unsigned char frameRate[1];
static unsigned char screenFilter[1];
static unsigned char aspectRatio[1];



class Program
{
public:
	class Invalid { public: std::string what{ "" }; };
};

