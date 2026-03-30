#pragma once
#include <iostream>

namespace PrintManager {

	void printOperInputErrorMsg() {
		std::cout << "잘못된 명령어 입력입니다.\n";
	}

	void printSuccess() {
		std::cout << "SUCCESS" << "\n";
	}

	void printError() {
		std::cout << "ERROR" << "\n";
	}
}