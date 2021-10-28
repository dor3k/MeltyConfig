#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Option.h"
#include "Program.h"

//TODO: Main program loop
//TODO: Builders
//TODO: Clean up the mess
//TODO: Better user interface
//TODO: Error handling

bool checkHeaderFile(std::basic_fstream<unsigned char>& fs) {
	unsigned char file[0x17B];

	fs.seekg(0);
	fs.read(file, 0x17B);

	if (!(file[0] == 0x04 && file[1] == 0xF0 && file[2] == 0xFF && file[3] == 0xF0))
		return false;
	else
		return true;
}

//Byte containers
unsigned char wholeFile[0x17B];
unsigned char cpuDifficulty[1];
unsigned char winCountArcade[1];
unsigned char damageLevel[1];
unsigned char timerSpeed[1];
unsigned char winCountVersus[1];
unsigned char saveReplay[1];
unsigned char bgmVolume[1];
unsigned char sfxVolume[1];
unsigned char characterFilter[1];
unsigned char stageAnimations[1];
unsigned char viewFps[1];
unsigned char frameRate[1];
unsigned char screenFilter[1];
unsigned char aspectRatio[1];

void populateByteContainers(std::basic_fstream<unsigned char>& fs) {
	fs.seekg(0x04);
	fs.read(cpuDifficulty, 1);
	fs.seekg(0x08);
	fs.read(winCountArcade, 1);
	fs.seekg(0x0C);
	fs.read(damageLevel, 1);
	fs.seekg(0x10);
	fs.read(timerSpeed, 1);
	fs.seekg(0x1C);
	fs.read(winCountVersus, 1);
	fs.seekg(0x28);
	fs.read(saveReplay, 1);
	fs.seekg(0x144);
	fs.read(bgmVolume, 1);
	fs.seekg(0x148);
	fs.read(sfxVolume, 1);
	fs.seekg(0x160);
	fs.read(characterFilter, 1);
	fs.seekg(0x164);
	fs.read(stageAnimations, 1);
	fs.seekg(0x168);
	fs.read(viewFps, 1);
	fs.seekg(0x16C);
	fs.read(frameRate, 1);
	fs.seekg(0x174);
	fs.read(screenFilter, 1);
	fs.seekg(0x178);
	fs.read(aspectRatio, 1);
}



int main()
try {
	int input{ -1 };

	std::basic_fstream<unsigned char> fs("System/_AAGameData.dat", std::ios::in | std::ios::out | std::ios::binary);
	if (!fs) 
		throw Program::Invalid{ "Cannot open ./System/_AAGameData.dat. \nMake sure the program is in the same directory as MBAA.exe \nAND that the System directory is NOT set to read only" };

	//Check the header file
	if (!checkHeaderFile(fs)) 
		throw Program::Invalid{ "Header file mismatch. \nThere is something wrong with System/_AAGameData.dat, make sure the file is not damaged and make sure the program is in the same directory as MBAA.exe" };

	//Option descriptions and values for the menu
	//Copied over from https://wiki.gbl.gg/w/Melty_Blood/MBAACC/Internals#System.2F_App.ini
	Option optionCpuDifficulty{ "CPU difficulty in single player modes. Defaults to 2 (normal)\n0 (easiest) to 4 (hardest)\n", std::vector<unsigned int>{0, 1, 2, 3, 4}, 0x04, fs, 2 };
	Option optionWinCountArcade{ "Wins required per game in single player modes. Defaults to 2\n1 to 3\n", std::vector<unsigned int>{1, 2, 3}, 0x08, fs, 2 };
	Option optionDamageLevel{ "Damage multiplier for all game modes. Defaults to 2 (normal) \n0 (lowest) to 4 (highest)\n", std::vector<unsigned int>{0, 1, 2, 3, 4}, 0x0C, fs, 2 };
	Option optionTimerSpeed{ "Speed of the round timer, 0 disables time outs. Defaults to 2 (normal)\n0 (infinity) to 4 (fastest)\n", std::vector<unsigned int>{0, 1, 2, 3, 4}, 0x10, fs, 2 };
	Option optionWinCountVersus{ "Wins required per game in versus mode. Defaults to 2 \n1 to 3\n", std::vector<unsigned int>{1, 2, 3}, 0x1C, fs, 2 };
	Option optionSaveReplay{ "Toggle for automatically saving replays after each game. Defaults to 0 (off)  \n0 (off) or 1 (on)\n", std::vector<unsigned int>{0, 1}, 0x28, fs, 0 };
	Option optionBgmVolume{ "Music volume. The in-game value is 20 minus this, with the exception of off which is 21. 20 might not be valid. Defaults to 10\n0 (loudest) to 21 (off)\n", std::vector<unsigned int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}, 0x144, fs, 10 };
	Option optionSfxVolume{ "Sound Effect and Voice volume. The in-game value is 20 minus this, with the exception of off which is 21. 20 might not be valid. Defaults to 10\n0 (loudest) to 21 (off)\n", std::vector<unsigned int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}, 0x148, fs, 10 };
	Option optionCharacterFilter{ "The character filter mode. Defaults to 2 (full) \n0 (off) 1 (edge) 2 (full) 3 (linear)\n", std::vector<unsigned int>{0, 1, 2, 3}, 0x160, fs, 2 };
	Option optionStageAnimations{ "Whether stage animations are present. Defaults to 1 (on) \n0 (off) or 1 (on)\n", std::vector<unsigned int>{0, 1}, 0x164, fs, 1 };
	Option optionViewFps{ "Whether the FPS counter is shown. Defaults to 0 (off) \n0 (off) or 1 (on)\n", std::vector<unsigned int>{0, 1}, 0x168, fs, 0 };
	Option optionFrameRate{ "Sets the speed of the game. Defaults to 0 (normal) \n0 (normal) or 1 (half)\n", std::vector<unsigned int>{0, 1}, 0x16C, fs, 0 };
	Option optionScreenFilter{ "Toggle for the screen filter. Defaults to 0 (off) \n0 (off) or 1 (on)\n", std::vector<unsigned int>{0, 1}, 0x174, fs, 0 };
	Option optionAspectRatio{ "The aspect ratio setting of the game. Overrides the value in _App.ini. Defaults to 0 (normal)\n0 (normal) 1 (auto) 2 (4:3) 3 (16:9) 4 (16:10) 5 (5:4) 6 (15:9)\n", std::vector<unsigned int>{0, 1, 2, 3, 4, 5, 6}, 0x178, fs, 0 };
	
	//TODO: Shitty main loop
	bool exit{ 0 };
	int option{ 0 };
	while (!exit) {
		populateByteContainers(fs);

		switch (input) {
		default:
			std::cin.clear();
			std::cout << "Melty Config v 0.1\n";
			std::cout <<
				"[1]  Difficulty\t\t : " << int(cpuDifficulty[0]) << "\n" <<
				"[2]  Win Count (Arcade)\t : " << int(winCountArcade[0]) << "\n" <<
				"[3]  Damage Level\t : " << int(damageLevel[0]) << "\n" <<
				"[4]  Timer Speed\t : " << int(timerSpeed[0]) << "\n" <<
				"[5]  Win Count (VS)\t : " << int(winCountVersus[0]) << "\n" <<
				"[6]  Replay Save\t : " << int(saveReplay[0]) << "\n" <<
				"[7]  BGM Volume\t\t : " << int(bgmVolume[0]) << "\n" <<
				"[8]  SFX Volume\t\t : " << int(sfxVolume[0]) << "\n" <<
				"[9]  Character Filter\t : " << int(characterFilter[0]) << "\n" <<
				"[10] Stage Animations\t : " << int(stageAnimations[0]) << "\n" <<
				"[11] View FPS\t\t : " << int(viewFps[0]) << "\n" <<
				"[12] Framerate\t\t : " << int(frameRate[0]) << "\n" <<
				"[13] Screen Filter\t : " << int(screenFilter[0]) << "\n" <<
				"[14] Aspect Ratio\t : " << int(aspectRatio[0]) << "\n"
				"[15] Default All\t " << "\n"
				"[0]  Exit\t\t  " << "\n>";
			std::cin >> input;

			//When in doubt, flush the buffer
			while (!std::cin) {
				throw Program::Invalid{ "Invalid input" };
				std::cin.clear();
				std::cin.ignore(100, '\n');
			}
			break;
		case 0:
			exit = 1;
			break;
		case 1:
			optionCpuDifficulty.initialize();
			input = 999;
			break;
		case 2:
			optionWinCountArcade.initialize();

			input = 999;
			break;
		case 3:
			optionDamageLevel.initialize();
			input = 999;
			break;
		case 4:
			optionTimerSpeed.initialize();
			input = 999;
			break;
		case 5:
			optionWinCountVersus.initialize();
			input = 999;
			break;
		case 6:
			optionSaveReplay.initialize();
			input = 999;
			break;
		case 7:
			optionBgmVolume.initialize();
			input = 999;
			break;
		case 8:
			optionSfxVolume.initialize();
			input = 999;
			break;
		case 9:
			optionCharacterFilter.initialize();
			input = 999;
			break;
		case 10:
			optionStageAnimations.initialize();
			input = 999;
			break;
		case 11:
			optionViewFps.initialize();
			input = 999;
			break;
		case 12:
			optionFrameRate.initialize();
			input = 999;
			break;
		case 13:
			optionScreenFilter.initialize();
			input = 999;
			break;
		case 14:
			optionAspectRatio.initialize();
			input = 999;
			break;
		case 15:
			std::string defaultOptionInput{ "" };
			std::cout << "\nAre you sure you want to set all settings to their default values?\n0 (no) or 1 (yes)\n>";
			std::cin >> defaultOptionInput;
			if (defaultOptionInput == "1") {
				optionAspectRatio.setToDefault();
				optionBgmVolume.setToDefault();
				optionCharacterFilter.setToDefault();
				optionCpuDifficulty.setToDefault();
				optionDamageLevel.setToDefault();
				optionFrameRate.setToDefault();
				optionSaveReplay.setToDefault();
				optionScreenFilter.setToDefault();
				optionSfxVolume.setToDefault();
				optionStageAnimations.setToDefault();
				optionTimerSpeed.setToDefault();
				optionViewFps.setToDefault();
				optionWinCountArcade.setToDefault();
				optionWinCountVersus.setToDefault();

				std::cout << "\nAll options were set to their default values\n";
			}
			input = 999;
			break;
		}
	}
	return 0;
}
catch (Option::Invalid& e) {
		std::string input;
		if (!std::cin) {
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}
		

		std::cerr << "\nUnchaught Option error.";
		std::cerr << e.what << "\nThe program will now terminate. Press any key. ";
		std::cin >> input;
		return 1;
}
catch (Program::Invalid& e) {
		std::string input;
		if (!std::cin) {
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}

		std::cerr << "\nUnchaught Program error.";
		std::cerr << e.what << "\nThe program will now terminate. Press any key. ";
		std::cin >> input;
		return 1;
}