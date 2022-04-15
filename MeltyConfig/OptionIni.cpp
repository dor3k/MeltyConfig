#include "OptionIni.h"

OptionIni::OptionIni(std::string description, std::pair<unsigned int, unsigned int> valuesRange, std::string attribute, std::string displayname, CSimpleIniCaseA& simpleinifs, SI_Error& rc, const char* pv_ini, unsigned int defaultValue)
	:pDesc{ description }, pValuesRange{ valuesRange }, pAttribute{ attribute }, pDisplayName{ displayname }, pSimpleIniFs{ simpleinifs }, pRc{ rc }, pIniPath{ pv_ini }, pDefaultValue{ defaultValue }
{
	if (description == "" || description == " ")
		throw Invalid{ "OptionIni Constructor: Description cannot be empty" };
	if (valuesRange.first >= valuesRange.second || valuesRange.second > 9999)
		throw Invalid{ "OptionBinary Construtor: Invalid values" };
	//TODO: Add better sanity checking
	if (attribute == "" || attribute.size() > 16)
		throw Invalid{ "OptionIni Construtor: Invalid attribute" };
	if (displayname == "")
		throw Invalid{ "OptionIni Construtor: Invalid displayName" };
	if (rc < 0)
		throw Invalid{ "OptionIni Construtor: RC is more than 0" };
}
OptionIni::OptionIni(std::string description, std::pair<unsigned int, unsigned int> valuesRange, std::string attribute, std::string displayname, CSimpleIniCaseA& simpleinifs, SI_Error& rc, const char* pv_ini, unsigned int defaultValue, bool no_clear)
	:pDesc{ description }, pValuesRange{ valuesRange }, pAttribute{ attribute }, pDisplayName{ displayname }, pSimpleIniFs{ simpleinifs }, pRc{ rc }, pIniPath{ pv_ini }, pDefaultValue{ defaultValue }, f_no_clear{no_clear} 
{
	if (description == "" || description == " ")
		throw Invalid{ "OptionIni Constructor: Description cannot be empty" };
	if (valuesRange.first >= valuesRange.second || valuesRange.second > 9999)
		throw Invalid{ "OptionBinary Construtor: Invalid values" };
	//TODO: Add better sanity checking
	if (attribute == "" || attribute.size() > 16)
		throw Invalid{ "OptionIni Construtor: Invalid attribute" };
	if (displayname == "")
		throw Invalid{ "OptionIni Construtor: Invalid displayName" };
	if (rc < 0)
		throw Invalid{ "OptionIni Construtor: RC is more than 0" };
}
void OptionIni::initialize()
{
	print();
	getInput();
	setValue();

	if (!f_no_clear) 
		system("cls");

	std::cout << "Value '" << pDisplayName << "' set to " << pUserInput << '\n';
}
void OptionIni::print() {
	std::cout << pDesc << '\n';
}
void OptionIni::getInput()
{
	int input{ 0 };
	std::cout << ">";

	std::cin >> input;
	//Ignore the reminder of the input after reading the first part to avoid unwanted behavior
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	if (!std::cin)
		throw Invalid{ "[ERROR]: OptionIni::getInput:\nstd::cin fail. Please insert valid input." };

	if (!(validateInput(input))) {
		throw Invalid{ "[ERROR]: OptionIni::getInput:\nInput out of range. Please insert valid input." };
	}
	else
		pUserInput = input;
}
bool OptionIni::validateInput(int& input) {
	if (input < pValuesRange.first || input > pValuesRange.second)
		return false;
	else
		return true;
}
void OptionIni::setToDefault() {
	//Casting for the SimpleIni stream
	const char* att = pAttribute.c_str(); //cast string as const char*
	std::string value = std::to_string(pDefaultValue); //convert int to string
	char const* ptrvalue = value.c_str();  //cast string as const char*

	pRc = pSimpleIniFs.SetValue("System", att, ptrvalue);
	if (pRc < 0)
		throw Invalid{ "[ERROR] OptionIni::setToDefault:\nCould not write to SimpleIni FS." };
	else
		//Save the file and keep it UTF-8
		pSimpleIniFs.SaveFile(pIniPath, false);
}
void OptionIni::setValue() {
	const char* att = pAttribute.c_str(); //cast string as const char*
	std::string input = std::to_string(pUserInput); //convert int to string
	char const* ptrinput = input.c_str();  //cast string as const char*

	pRc = pSimpleIniFs.SetValue("System", att, ptrinput);
	if (pRc < 0)
		throw Invalid{ "[ERROR] OptionIni::setToDefault:\nCould not write to SimpleIni FS." };
	else
		//Save the file and keep it UTF-8
		pSimpleIniFs.SaveFile(pIniPath, false);
}