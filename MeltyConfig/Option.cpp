#include "Option.h"
//TODO: Add default value error checking
Option::Option(std::string description, std::vector<unsigned int> values, unsigned int offset, std::basic_fstream<unsigned char>& filestream, unsigned int defaultValue)
	:pDesc{ description }, pValues{ values }, pOffset{ offset }, pFileStream{ filestream }, pDefaultValue{ defaultValue } {
	if (description == "" || description == " ")
		throw Invalid{ "Option Constructor: Description cannot be empty" };
	for (auto n : values) {
		if (n < 0 || values.size() == 0)
			throw Invalid{ "Option Construtor: Invalid values" };
	}
	if (offset < 0x04 || offset > 0x178)
		throw Invalid{ "Option Construtor: Invalid offset" };
	if (!filestream)
		throw Invalid{ "Option Construtor: Invalid filestream" };

}
void Option::initialize()
{
	print();
	getInput();
	setValue();

	std::cout << "\nValue set to " << pUserInput << " at offset 0x" << pOffset << '\n';
}
void Option::print(){
	std::cout << pDesc << '\n';
}
void Option::getInput()
{
	int input{ 0 };
	std::cout << ">";
	
	std::cin >> input;
	if (!std::cin)
		throw Invalid{ "Get Value: Invalid Input" };

	if (!(validateInput(input))) {
		throw Invalid{ "Get Value: Invalid Input, input out of range." };
	}
	else
		pUserInput = input;
}
bool Option::validateInput(int& input){
	for (auto n : pValues)
		if (input == n)
			return true;

	return false;
}
void Option::setToDefault(){
	unsigned char value[1]{ pDefaultValue };
	pFileStream.seekp(pOffset);
	pFileStream.write(value, 1);
}
void Option::setValue(){
	unsigned char value[1]{ pUserInput };
	pFileStream.seekp(pOffset);
	pFileStream.write(value, 1);
}
