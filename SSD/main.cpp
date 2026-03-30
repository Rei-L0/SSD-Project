#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

const int LBA_SIZE = 4;
const int MIN_LBA_SIZE = 0;
const int MAX_LBA_SIZE = 100;
const int LINE_SIZE = 10;

const std::string DEFALT_DATA = "00000000";

const std::string FILE_NAME = "nand.txt";

std::string parseValue(std::string s) {
	return s.substr(1);
}

bool read(std::fstream file, int idx) {
	return true;
}

bool write(std::fstream& file, int idx, std::string value) {
	file.seekp(idx*LINE_SIZE);
	file << parseValue(value) << "\n";
	return true;
}

void init(std::ofstream &fout) {
	for (size_t i = 0; i < MAX_LBA_SIZE; i++)
	{
		fout << DEFALT_DATA << "\n";
	}
}

void printOperInputErrorMsg() {
	std::cout << "잘못된 명령어 입력입니다.\n";
}

bool validLbaIdx(int idx) {
	return MIN_LBA_SIZE <= idx && idx < MAX_LBA_SIZE;
}

void printSuccess() {
	std::cout << "SUCCESS";
}

void printError() {
	std::cout << "ERROR";
}

int main() {
	if (!std::filesystem::exists(FILE_NAME)) {
		std::ofstream fout(FILE_NAME);
		if (fout.is_open()) {
			init(fout);
			std::cout << "초기화 완료\n";
		}
	}

	while (true) {
		std::fstream file(FILE_NAME,std::ios::in||std::ios::out);
		
		char op = 'a';
		int lba_idx = 0;
		std::cin >> op >> lba_idx;

		if (op == 'W') {
			if (!validLbaIdx(lba_idx)) {
				printError();
				continue;
			}
			std::string value;
			std::cin >> value;

			if (write(file,lba_idx,value)) {
				printSuccess();
			}
		}
		else if (op == 'R') {
			if (!validLbaIdx(lba_idx)) {
				printError();
				continue;
			}

		}
		else {
			printOperInputErrorMsg();
		}

		file.close();
		if (op != 'W' && op != 'R')break;
	}
}