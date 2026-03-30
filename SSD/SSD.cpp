#include <iostream>
#include <fstream>
#include <string>
#include "ssd.h"


SSD::SSD() {
	file.open(FILE_NAME, std::ios::in | std::ios::out);
}

SSD::~SSD() {
	file.close();
}

std::string SSD::parseValue(std::string s) {
	return s.substr(2);
}

bool SSD::validLbaIdx(int idx) {
	return MIN_LBA_SIZE <= idx && idx < MAX_LBA_SIZE;
}

bool SSD::read(int idx, std::string& msg) {
	if (!file.is_open()) return false;
	int lineNum = 0;
	std::string s;
	while (!file.eof()) {
		std::getline(file, s);
		if (lineNum == idx) {
			std::cout << "0x" << s << "\n";
			break;
		}
		lineNum++;
	}
	return true;
}

bool SSD::write(int idx, std::string value) {
	if (!file.is_open()) return false;

	std::string s = parseValue(value);

	if (s.length() != 8) {
		return false;
	}

	file.seekp(idx * LINE_SIZE, std::ios::beg);
	file << s;
	return true;
}

void SSD::init(std::ofstream& fout) {
	for (size_t i = 0; i < MAX_LBA_SIZE; i++)
	{
		fout << DEFALT_DATA << "\n";
	}
}

