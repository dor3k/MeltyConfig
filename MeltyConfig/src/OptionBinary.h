#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "Program.h"

class OptionBinary
{
public:
	OptionBinary(std::string description, std::pair<unsigned int, unsigned int> valuesRange, unsigned int offset, std::fstream& filestream, unsigned int defaultValue);
	OptionBinary(std::string description, std::pair<unsigned int, unsigned int> valuesRange, unsigned int offset, std::fstream& filestream, unsigned int defaultValue, bool convertMusicValues);
	class Invalid { public: std::string what{ "" }; };
	void initialize();
	void setToDefault();

	std::string getDescription() const { return pDesc; }
	std::pair<unsigned int, unsigned int> getValuesRange() const { return pValuesRange; }
	unsigned int offset() const { return pOffset; }
private:
	int convertMusicValues(int value);
	void print();
	void setValue();
	void getInput();
	bool validateInput(int& input);
	
	std::string pDesc;
	std::pair<unsigned int, unsigned int> pValuesRange;
	std::fstream& pFileStream;
	const unsigned int pOffset{ 0x0 };
	unsigned int pUserInput{ 0 };
	unsigned int pDefaultValue{ 0x0 };
	bool pFlagConvertMusicValues{ 0 };
};

