#include <iostream>
#include <fstream>
#include <string>
#include "FileSystem.h"


FS::FS() {

	file.open(FILE_NAME, std::ios::in | std::ios::out | std::ios::trunc);
}

FS::~FS() {
	file.close();
}

bool FS::write(std::string msg) {
	if (!file.is_open()) return false;

	file.seekg(0, std::ios::end);
	file << msg << "\n";
	return true;
}