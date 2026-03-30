#pragma once
#pragma once
#include <string>
#include <fstream>

class FS {
public:
	const std::string FILE_NAME = "output.txt";

	std::fstream file;

	FS();
	~FS();

	bool write(std::string value);
};