#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Option
{
public:
	Option(std::string description, std::vector<unsigned int> values, unsigned int offset, std::basic_fstream<unsigned char>& filestream, unsigned int defaultValue);
	class Invalid { public: std::string what{ "" }; };
	void initialize();
	void setToDefault();

	std::string getDescription() const { return pDesc; }
	std::vector<unsigned int> getValues() const { return pValues; }
	unsigned int offset() const { return pOffset; }
private:
	void print();
	void setValue();
	void getInput();
	bool validateInput(int& input);

	std::string pDesc;
	std::vector<unsigned int> pValues;
	std::basic_fstream<unsigned char>& pFileStream;
	const unsigned int pOffset{ 0x0 };
	unsigned int pUserInput{ 0 };
	unsigned int pDefaultValue{ 0x0 };
};

