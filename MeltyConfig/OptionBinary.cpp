#include "OptionBinary.h"

OptionBinary::OptionBinary(std::string description, std::pair<unsigned int, unsigned int> valuesRange, unsigned int offset, std::basic_fstream<unsigned char>& filestream, unsigned int defaultValue)
	:pDesc{ description }, pValuesRange{ valuesRange }, pOffset{ offset }, pFileStream{ filestream }, pDefaultValue{ defaultValue }, pFlagConvertMusicValues {0}
 {
	if (description == "" || description == " ")
		throw Invalid{ "OptionBinary Constructor: Description cannot be empty" };
	if (valuesRange.first >= valuesRange.second || valuesRange.second > 9999)
		throw Invalid{ "OptionBinary Construtor: Invalid values" };
	if (offset < 0x04 || offset > 0x178)
		throw Invalid{ "OptionBinary Construtor: Invalid offset" };
	if (!filestream)
		throw Invalid{ "OptionBinary Construtor: Invalid filestream" };

}
OptionBinary::OptionBinary(std::string description, std::pair<unsigned int, unsigned int> valuesRange, unsigned int offset, std::basic_fstream<unsigned char>& filestream, unsigned int defaultValue, bool convertMusicValues)
	:pDesc{ description }, pValuesRange{ valuesRange }, pOffset{ offset }, pFileStream{ filestream }, pDefaultValue{ defaultValue }, pFlagConvertMusicValues{ convertMusicValues }
{
	if (description == "" || description == " ")
		throw Invalid{ "OptionBinary Constructor: Description cannot be empty" };
	if (valuesRange.first >= valuesRange.second || valuesRange.second > 9999)
		throw Invalid{ "OptionBinary Construtor: Invalid values" };
	if (offset < 0x04 || offset > 0x178)
		throw Invalid{ "OptionBinary Construtor: Invalid offset" };
	if (!filestream)
		throw Invalid{ "OptionBinary Construtor: Invalid filestream" };
}
void OptionBinary::initialize()
{
	print();
	getInput();
	setValue();

	system("cls");
	std::cout << "INFO: Value set to 0x" << std::hex << pUserInput << " at offset 0x" << pOffset << std::dec << '\n';
}
int OptionBinary::convertMusicValues(int value)
{
	// Converts the desired value of music to a value in game's format
	// 21 == OFF, 20 invalid, Rest == 20 - value. Max value = 0x0 (ingame 20)
	// Example ingame 7 == 20 - 7 = 13 (0x0D)
	// Melty Blood is weird
	if (value <= 0 || value > 20)
		return 21;
	else
		return 20 - value;
}
void OptionBinary::print(){
	std::cout << pDesc << '\n';
	std::cout << "Q to abort\n";
}
void OptionBinary::getInput()
{
	int input{ 0 };
	std::cout << ">";
	std::string rawInput{ "" };

	std::cin >> rawInput;
	//Ignore the reminder of the input after reading the first part to avoid unwanted behavior
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	//Check for abort symbol 'q' and check for std::stoi errors
	if ((rawInput[0] == 'q' || rawInput[0] == 'Q') && rawInput.length() == 1)
		throw Invalid{ "[INFO]: OptionBinary::getInput:\nOperation aborted by the user." };
	else {
		try {
			input = std::stoi(rawInput);
		}
		catch (...) {
			throw Invalid{ "[ERROR]: OptionBinary::getInput:\nstd::stoi fail. Please insert valid input." };
		}
	}
	if (!std::cin)
		throw Invalid{ "[ERROR]: OptionBinary::getInput:\nstd::cin fail. Please insert valid input." };
	if (!(validateInput(input))) {
		throw Invalid{ "[ERROR]: OptionBinary::getInput:\nInput out of range. Please insert valid input." };
	}
	else if (pFlagConvertMusicValues)
		pUserInput = convertMusicValues(input);
	else
		pUserInput = input;
}
bool OptionBinary::validateInput(int& input){
	if (input < pValuesRange.first || input > pValuesRange.second)
			return false;
	else
		return true;
}
//Conversion to binary done by filestream std::ios::binary
//If filestrean would be open without the flag the values could be incorrect
void OptionBinary::setToDefault(){
	unsigned char value[1]{ pDefaultValue };
	pFileStream.seekp(pOffset);
	pFileStream.write(value, 1);
}
void OptionBinary::setValue(){
	unsigned char value[1]{ pUserInput };
	pFileStream.seekp(pOffset);
	pFileStream.write(value, 1);
}
