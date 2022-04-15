#pragma once
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include "SimpleIni.h"

class OptionIni
{
public:
	OptionIni(std::string description, std::pair<unsigned int, unsigned int> valuesRange, std::string attribute, std::string displayname, CSimpleIniCaseA& simpleinifs, SI_Error& rc, const char* pv_ini, unsigned int defaultValue);
	OptionIni(std::string description, std::pair<unsigned int, unsigned int> valuesRange, std::string attribute, std::string displayname, CSimpleIniCaseA& simpleinifs, SI_Error& rc, const char* pv_ini, unsigned int defaultValue, bool no_clear);
	class Invalid { public: std::string what{ "" }; };
	void initialize();
	void setToDefault();

	std::string getDescription() const { return pDesc; }
	std::pair<unsigned int, unsigned int> getValuesRange() const { return pValuesRange; }
	std::string getAttribute() const { return pAttribute; }
	std::string getDisplayName() const { return pDisplayName; }
private:
	void print();
	void setValue();
	void getInput();
	bool validateInput(int& input);

	std::string pDesc;
	std::string pDisplayName;
	std::pair<unsigned int, unsigned int> pValuesRange;
	std::string pAttribute;
	CSimpleIniCaseA& pSimpleIniFs;
	SI_Error& pRc;
	bool f_no_clear{0};
	const char* pIniPath;
	unsigned int pUserInput{ 0 };
	unsigned int pDefaultValue{ 0 };
};

