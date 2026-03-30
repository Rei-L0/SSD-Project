// server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "SSD.h"

using boost::asio::ip::tcp;

void printOperInputErrorMsg() {
	std::cout << "잘못된 명령어 입력입니다.\n";
}

void printSuccess() {
	std::cout << "SUCCESS" << "\n";
}

void printError() {
	std::cout << "ERROR" << "\n";
}

int main() {

	try {
		boost::asio::io_context io;
		SSD ssd;

		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));

		std::cout << "server started on port 12345\n";

		while (true) {
			tcp::socket socket(io);
			acceptor.accept(socket);

			std::cout << "client connected\n";

			while (true) {
				char data[1024];

				boost::system::error_code ec;
				size_t length = socket.read_some(boost::asio::buffer(data), ec);

				if (ec == boost::asio::error::eof)
					break; // 정상 종료
				else if (ec)
					throw boost::system::system_error(ec);

				// echo

				std::string cmd(data);
				cmd.erase(cmd.begin() + length, cmd.end());
				std::stringstream ss(cmd);
				std::vector<std::string> cmds;

				std::string tem;

				while (std::getline(ss, tem, ' ')) {
					if (tem != "") {
						cmds.push_back(tem);
					}
				}


				char op = data[0];
				int lba_idx = std::stoi(cmds[1]);
				std::string msg;
				if (op == 'W') {
					std::string value;
					value = cmds[2];
					if (!ssd.validLbaIdx(lba_idx)) {
						printError();
						continue;
					}

					if (ssd.write(lba_idx, value)) {
						printSuccess();
					}
					else {
						printError();
					}
				}
				else if (op == 'R') {
					if (!ssd.validLbaIdx(lba_idx)) {
						printError();
						continue;
					}
					if (ssd.read(lba_idx, msg)) {
						strcpy_s(data, msg.c_str());
					}
					else {
						printError();
					}
				}

				boost::asio::write(socket, boost::asio::buffer(data, length));

				if (op != 'W' && op != 'R')break;
			}

			std::cout << "client disconnected\n";
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}