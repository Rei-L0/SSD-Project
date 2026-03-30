#pragma once
#include <string>
#include <fstream>

class SSD {
public:
	const int LBA_SIZE = 4;
	const int MIN_LBA_SIZE = 0;
	const int MAX_LBA_SIZE = 100;
	const int LINE_SIZE = 10;

	const std::string DEFALT_DATA = "00000000";
	const std::string FILE_NAME = "nand.txt";

	std::fstream file;

	SSD();
	~SSD();

	std::string parseValue(std::string s);
	bool validLbaIdx(int idx);
	bool read( int idx, std::string& msg);
	bool write(int idx, std::string value);
	void init(std::ofstream& fout);
};

