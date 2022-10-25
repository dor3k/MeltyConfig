#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <utility>
#include <limits>
#include <tchar.h>
#include "OptionBinary.h"
#include "OptionIni.h"
#include "Program.h"
#include "SimpleIni.h"

//program title macro
constexpr auto PROGRAM_TITLE = "Melty Config v0.4\n";


///////////////////////////////////
// 
// Melty Config v0.4
// Created by dor3k
// https://github.com/dor3k/MeltyConfig
//
// - Bugfixes
// - Volume Options are now displayed and inputted in a logical way reflecting their in-game values instead of their internal ones
// - Added the option to abort changing values
// 
///////////////////////////////////

//TODO: Main program loop
//TODO: Builders
//TODO: Better program structure, move option methods to different classes
//TODO: Byte container struct

bool checkAAGameDataHeaderFile(std::basic_fstream<unsigned char>& fs) {
	unsigned char file[0x17B];

	fs.seekg(0);
	fs.read(file, 0x17B);

	if (file[0] == 0x04 && file[1] == 0xF0 && file[2] == 0xFF && file[3] == 0xF0)
		return true;
	else
		return false;
}

void getReadableOptionValues(std::basic_fstream<unsigned char>& fs) {
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

	//Required conversion to display the .dat values in a human-readable manner
	//Ingame values is equal to .dat Volume Value - 20
	//With .dat Volume Value 21 being ingame off
	bgmVolume[0] = 20 - bgmVolume[0];
	if (bgmVolume[0] > 20)
		bgmVolume[0] = 0;
	sfxVolume[0] = 20 - sfxVolume[0];
	if (sfxVolume[0] > 20)
		sfxVolume[0] = 0;
}

std::fstream openAppIniFileStream(std::string location) {
	std::fstream fs_ini(location);
	if (!fs_ini)
		throw Program::Invalid{ "Cannot open ./System/_App.ini \nMake sure the program is in the same directory as MBAA.exe\nor MeltyConfig.ini uses the correct path to _App.ini." };
	return  fs_ini;
}

bool validateAAGameDataFileStream(std::basic_fstream<unsigned char>& fs) {
	if (!fs) {
		throw Program::Invalid{ "Cannot open ./System/_AAGameData.dat. \nMake sure the program is in the same directory as MBAA.exe\nor MeltyConfig.ini uses the correct path to _AAGameData.dat." };
		return false;
	}
	if (!checkAAGameDataHeaderFile(fs)) {
		throw Program::Invalid{ "\nHeader file mismatch. \nThere is something wrong with System/_AAGameData.dat, make sure the file is not damaged and make sure the program is in the same directory as MBAA.exe" };
		return false;
	}
	return true;
}

std::vector<std::pair<std::string, int>> parseAppIniStream(std::fstream& fs) {
	std::vector<std::pair<std::string, int>> v;
	while (fs) {
		std::string parameter;
		std::getline(fs, parameter, '=');
		int value;
		fs >> value;
		fs.get(); //Discard the new line character
		if (!fs)
			break;
		v.push_back(std::make_pair(parameter, value));
	}
	fs.seekp(0);
	return v;
}


int main()
try {
	SetConsoleTitle(_T("Melty Config 0.4"));

	//Create MeltyConfig.ini containing path to System/
	//If not defined assumes a default path of ./System/
	CSimpleIniCaseA mainini;
	mainini.SetUnicode();
	SI_Error rcmainini = mainini.LoadFile("MeltyConfig.ini");
	if (rcmainini < 0) {
		std::cerr << "MeltyConfig.ini not found, creating a new file\n";
		mainini.SetValue("MeltyConfig", "App_ini_Path", "System/_App.ini");
		mainini.SetValue("MeltyConfig", "AAGameData_dat_Path", "System/_AAGameData.dat");
		rcmainini = mainini.SaveFile("MeltyConfig.ini");
	};

	//Ptr to System/ files path value 
	const char* pv_ini_path;
	pv_ini_path = mainini.GetValue("MeltyConfig", "App_ini_Path");
	const char* pv_aagamedata_path;
	pv_aagamedata_path = mainini.GetValue("MeltyConfig", "AAGameData_dat_Path");

	//fstream for reading from and writing to AAGameData.dat
	std::basic_fstream<unsigned char> fs(pv_aagamedata_path, std::ios::in | std::ios::out | std::ios::binary);
	if (!validateAAGameDataFileStream(fs))
		return 11;

	//SimpleIni App.ini stream
	CSimpleIniCaseA app_ini;
	app_ini.SetUnicode();
	SI_Error app_ini_rc = app_ini.LoadFile(pv_ini_path);
	if (app_ini_rc < 0) {
		throw Program::Invalid{ "Cannot open _App.ini\nMake sure no other program is currently using the file\n" };
	}

	//Option descriptions and values for the menu
	//Copied over from https://wiki.gbl.gg/w/Melty_Blood/MBAACC/Internals#System.2F_App.ini
	OptionBinary optionCpuDifficulty{ "CPU difficulty in single player modes. Defaults to 2 (normal)\n0 (easiest) to 4 (hardest)\n", std::pair<unsigned int, unsigned int>{0, 4}, 0x04, fs, 2 };
	OptionBinary optionWinCountArcade{ "Wins required per game in single player modes. Defaults to 2\n1 to 3\n", std::pair<unsigned int, unsigned int>{1, 3}, 0x08, fs, 2 };
	OptionBinary optionDamageLevel{ "Damage multiplier for all game modes. Defaults to 2 (normal) \n0 (lowest) to 4 (highest)\n", std::pair<unsigned int, unsigned int>{0, 4}, 0x0C, fs, 2 };
	OptionBinary optionTimerSpeed{ "Speed of the round timer, 0 disables time outs. Defaults to 2 (normal)\n0 (infinity) to 4 (fastest)\n", std::pair<unsigned int, unsigned int>{0, 4}, 0x10, fs, 2 };
	OptionBinary optionWinCountVersus{ "Wins required per game in versus mode. Defaults to 2 \n1 to 3\n", std::pair<unsigned int, unsigned int>{1, 3}, 0x1C, fs, 2 };
	OptionBinary optionSaveReplay{ "Toggle for automatically saving replays after each game. Defaults to 0 (off)  \n0 (off) or 1 (on)\n", std::pair<unsigned int, unsigned int>{0, 1}, 0x28, fs, 0 };
	OptionBinary optionBgmVolume{ "Music volume. Defaults to 10\n0 (off) to 20 (loudest)\n", std::pair<unsigned int, unsigned int>{0, 20}, 0x144, fs, 10, true};
	OptionBinary optionSfxVolume{ "Sound Effect and Voice volume. Defaults to 10\n0 (off) to 20 (loudest)\n", std::pair<unsigned int, unsigned int>{0, 20}, 0x148, fs, 10, true};
	OptionBinary optionCharacterFilter{ "The character filter mode. Defaults to 2 (full) \n0 (off) 1 (edge) 2 (full) 3 (linear)\n", std::pair<unsigned int, unsigned int>{0, 3}, 0x160, fs, 2 };
	OptionBinary optionStageAnimations{ "Whether stage animations are disabled or not. Defaults to 0 (no)\n0 (no) or 1 (yes)\n", std::pair<unsigned int, unsigned int>{0, 1}, 0x164, fs, 0 };
	OptionBinary optionViewFps{ "Whether the FPS counter is shown. Defaults to 0 (off) \n0 (off) or 1 (on)\n", std::pair<unsigned int, unsigned int>{0, 1}, 0x168, fs, 0 };
	OptionBinary optionFrameRate{ "Sets the speed of the game. Defaults to 0 (normal) \n0 (normal) or 1 (half)\n", std::pair<unsigned int, unsigned int>{0, 1}, 0x16C, fs, 0 };
	OptionBinary optionScreenFilter{ "Toggle for the screen filter. Defaults to 0 (off) \n0 (off) or 1 (on)\n", std::pair<unsigned int, unsigned int>{0, 1}, 0x174, fs, 0 };
	OptionBinary optionAspectRatio{ "The aspect ratio setting of the game. Overrides the value in _App.ini. Defaults to 1 (normal)\n0 (normal) 1 (auto) 2 (4:3) 3 (16:9) 4 (16:10) 5 (5:4) 6 (15:9)\n", std::pair<unsigned int, unsigned int>{0, 6}, 0x178, fs, 1 };
	
	OptionIni optionNoVsMovie{ "Use simple vs mode screen. Defaults to 0 (off)\n0 (off) or 1 (on)\n", std::pair<unsigned int, unsigned int>{0, 1}, "NoVsMovie", "No Versus Movie", app_ini, app_ini_rc, pv_ini_path, 0 };
	OptionIni optionWindowed{ "Start the game in windowed mode. Defaults to 0 (fullscreen)\n0 (full screen) or 1 (windowed)\n", std::pair<unsigned int, unsigned int>{0, 1}, "Windowed", "Windowed Mode", app_ini, app_ini_rc, pv_ini_path, 0 };
	OptionIni optionScreenW{ "Width of the game screen/window in pixels. Defaults to 800\n1 to 9999\n", std::pair<unsigned int, unsigned int>{1, 9999}, "ScreenW", "Screen Width", app_ini, app_ini_rc, pv_ini_path, 800, 1 };
	OptionIni optionScreenH{ "Height of the game screen/window in pixels. Defaults to 600\n1 to 9999\n", std::pair<unsigned int, unsigned int>{1, 9999}, "ScreenH", "Screen Height", app_ini, app_ini_rc, pv_ini_path, 600 };
	OptionIni optionPosX{ "Position of the left edge of the game window in pixels relative to the left edge of the screen.\nIgnored in full screen mode. Defaults to 0\n1 to 9999", std::pair<unsigned int, unsigned int>{0, 9999}, "PosX", "Window Position X", app_ini, app_ini_rc, pv_ini_path, 0, 1 };
	OptionIni optionPosY{ "Position of the top edge of the game window in pixels relative to the top edge of the screen.\nIgnored in full screen mode. Defaults to 0\n1 to 9999", std::pair<unsigned int, unsigned int>{0, 9999}, "PosY", "Window Position Y", app_ini, app_ini_rc, pv_ini_path, 0 };
	//	TODO: Write a proper main loop
	// 	This one works but the code is ugly
	//	I suck at making interfaces
	bool exit{ 0 };
	char input{ '1' };
	int programWindow{ 0 };

	while (!exit) {
		try {
			//Catch std::cin failing and reset its state
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			}
			if (programWindow == 0) {
				getReadableOptionValues(fs);
				std::cout << PROGRAM_TITLE;
				std::cout <<
					"[1] Difficulty\t\t\t : " << int(cpuDifficulty[0]) << "\n" <<
					"[2] Win Count (Arcade)\t\t : " << int(winCountArcade[0]) << "\n" <<
					"[3] Damage Level\t\t : " << int(damageLevel[0]) << "\n" <<
					"[4] Timer Speed\t\t\t : " << int(timerSpeed[0]) << "\n" <<
					"[5] Win Count (VS)\t\t : " << int(winCountVersus[0]) << "\n" <<
					"[6] Replay Save\t\t\t : " << int(saveReplay[0]) << "\n" <<
					"[7] BGM Volume\t\t\t : " << int(bgmVolume[0]) << "\n" <<
					"[8] SFX Volume\t\t\t : " << int(sfxVolume[0]) << "\n" <<
					"[9] Character Filter\t\t : " << int(characterFilter[0]) << "\n" <<
					"[A] Disable Stage Animations\t : " << int(stageAnimations[0]) << "\n" <<
					"[B] View FPS\t\t\t : " << int(viewFps[0]) << "\n" <<
					"[C] Framerate\t\t\t : " << int(frameRate[0]) << "\n" <<
					"[D] Screen Filter\t\t : " << int(screenFilter[0]) << "\n" <<
					"[E] Aspect Ratio\t\t : " << int(aspectRatio[0]) << "\n"
					"[F] Default All\t\t " << "\n"
					"[N] Next Page\t\t" << "\n"
					"[0] Exit\t\t\t  " << "\n>";

				input = _getch();
				input = toupper(input);

				switch (input) {
				default:
					system("cls");
					break;
				case '0': case ')':
					exit = 1;
					break;
				case '1': case '!':
					optionCpuDifficulty.initialize();
					break;
				case '2': case '@':
					optionWinCountArcade.initialize();
					break;
				case '3': case '#':
					optionDamageLevel.initialize();
					break;
				case '4': case '$':
					optionTimerSpeed.initialize();
					break;
				case '5': case '%':
					optionWinCountVersus.initialize();
					break;
				case '6': case '^':
					optionSaveReplay.initialize();
					break;
				case '7': case '&':
					optionBgmVolume.initialize();
					break;
				case '8': case '*':
					optionSfxVolume.initialize();
					break;
				case '9': case '(':
					optionCharacterFilter.initialize();
					break;
				case 'A':
					optionStageAnimations.initialize();
					break;
				case 'B':
					optionViewFps.initialize();
					break;
				case 'C':
					optionFrameRate.initialize();
					break;
				case 'D':
					optionScreenFilter.initialize();
					break;
				case 'E':
					optionAspectRatio.initialize();
					break;
				case 'F': {
					std::string caseDefaultInput{ "" };

					std::cout << "\nAre you sure you want to set all settings on this page to their default values?\n0 (no) or 1 (yes)\n>";
					std::cin >> caseDefaultInput;
					std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

					system("cls");
					if (caseDefaultInput == "1") {
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

						std::cout << "All options were set to their default values\n";
					}
					break;
				}
				case 'N':
					system("cls");
					programWindow = 1;
					break;
				}
			}
			if (programWindow == 1) {
				auto fs_ini{ openAppIniFileStream(pv_ini_path) };
				std::vector<std::pair<std::string, int>> vIni{ parseAppIniStream(fs_ini) };
				std::cout << PROGRAM_TITLE;
				std::cout <<
					"[1] Resolution\t\t : " << vIni[2].second << "x" << vIni[3].second << "\n" <<
					"[2] Window Position\t : " << vIni[5].second << "x" << vIni[6].second << "\n" <<
					"[3] No Versus Movie\t : " << vIni[0].second << "\n" <<
					"[4] Windowed Mode\t : " << vIni[1].second << "\n" <<
					"[F] Default All\t " << "\n"
					"[N] Previous Page\t" << "\n"
					"[0] Exit\t\t  " << "\n>";
				fs_ini.close();
				input = _getch();
				input = toupper(input);

				switch (input) {
				default:
					system("cls");
					break;
				case '0': case ')':
					exit = 1;
					break;
				case '1': case '!':
					optionScreenW.initialize();
					optionScreenH.initialize();
					break;
				case '2': case '@':
					optionPosX.initialize();
					optionPosY.initialize();
					break;
				case '3': case '#':
					optionNoVsMovie.initialize();
					break;
				case '4': case '$':
					optionWindowed.initialize();
					break;
				case 'F': {
					std::string caseDefaultInput{ "" };

					std::cout << "\nAre you sure you want to set all settings on this page to their default values?\n0 (no) or 1 (yes)\n>";
					std::cin >> caseDefaultInput;
					std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

					system("cls");
					if (caseDefaultInput == "1") {
						optionNoVsMovie.setToDefault();
						optionScreenW.setToDefault();
						optionScreenH.setToDefault();
						optionPosX.setToDefault();
						optionPosY.setToDefault();
						optionWindowed.setToDefault();
						std::cout << "All options were set to their default values\n";
					}
					break;
				}
					
				case 'N':
					system("cls");
					programWindow = 0;
					break;
				}
			}
		}
		catch (Program::Invalid& e){
			system("cls");
			std::cerr << "Exception caught: Program error\n" << e.what << '\n' << '\n';
		}

		catch (OptionBinary::Invalid& e) {
			system("cls");
			std::cerr << e.what << '\n';
		}
		catch (OptionIni::Invalid& e) {
			system("cls");
			std::cerr << e.what << '\n';
		}
	}
	std::cout << "Done";
	return 0;

}
catch (OptionBinary::Invalid& e) {
		std::string input;
		if (!std::cin) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
	
		std::cerr << "\nUnchaught expection: OptionBinary error.";
		std::cerr << e.what << "\nThe program will now terminate. Press any key. ";
		input = _getch();
		return 1;
}
catch (OptionIni::Invalid& e) {
	std::string input;
	if (!std::cin) {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}

	std::cerr << "\nUnchaught expection: OptionIni error.";
	std::cerr << e.what << "\nThe program will now terminate. Press any key. ";
	input = _getch();
	return 1;
}
catch (Program::Invalid& e) {
		std::string input;
		if (!std::cin) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}

		std::cerr << "\nUnchaught exception: Program error.";
		std::cerr << e.what << "\nThe program will now terminate. Press any key. ";
		input = _getch();
		return 1;
}
catch (std::exception& e) {

	std::string input;
	if (!std::cin) {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}

	std::cerr << "\nUnchaught exception: std::exception";
	std::cerr << e.what();
	std::cerr << "\nThe program will now terminate. Press any key. ";
	input = _getch();
	return 1;
}