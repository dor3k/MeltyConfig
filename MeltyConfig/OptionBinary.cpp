#include "OptionBinary.h"

//TODO: Add default value error checking
OptionBinary::OptionBinary(std::string description, std::pair<unsigned int, unsigned int> valuesRange, unsigned int offset, std::basic_fstream<unsigned char>& filestream, unsigned int defaultValue)
	:pDesc{ description }, pValuesRange{ valuesRange }, pOffset{ offset }, pFileStream{ filestream }, pDefaultValue{ defaultValue } {
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
	std::cout << "Value set to " << pUserInput << " at offset 0x" << pOffset << '\n';
}
void OptionBinary::print(){
	std::cout << pDesc << '\n';
}
void OptionBinary::getInput()
{
	int input{ 0 };
	std::cout << ">";
	
	std::cin >> input;
	//Ignore the reminder of the input after reading the first part to avoid unwanted behavior
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	if (!std::cin)
		throw Invalid{ "[ERROR]: OptionBinary::getInput:\nstd::cin fail. Please insert valid input." };

	if (!(validateInput(input))) {
		throw Invalid{ "[ERROR]: OptionBinary::getInput:\nInput out of range. Please insert valid input." };
	}
	else
		pUserInput = input;
}
bool OptionBinary::validateInput(int& input){
	if (input < pValuesRange.first || input > pValuesRange.second)
			return false;
	else
		return true;
}
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
