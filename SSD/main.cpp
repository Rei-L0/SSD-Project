// server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

using boost::asio::ip::tcp;

const int lba_size = 4;
const int min_lba_size = 0;
const int max_lba_size = 100;
const int line_size = 10;

const std::string defalt_data = "00000000";

const std::string file_name = "nand.txt";

std::string parsevalue(std::string s) {
	return s.substr(2);
}

bool validlbaidx(int idx) {
	return min_lba_size <= idx && idx < max_lba_size;
}

bool read(std::fstream& file, int idx, std::string& msg) {
	if (!file.is_open()) return false;

	int linenum = 0;
	std::string s;
	while (!file.eof()) {
		std::getline(file, s);
		if (linenum == idx) {
			std::cout << "0x" << s << "\n";
			break;
		}
		linenum++;
	}
	msg = s;
	return true;
}

bool write(std::fstream& file, int idx, std::string value) {
	if (!file.is_open()) return false;

	std::string s = parsevalue(value);

	if (s.length() != 8) {
		return false;
	}

	file.seekp(idx * line_size, std::ios::beg);
	file << s;
	return true;
}

void init(std::ofstream& fout) {
	for (size_t i = 0; i < max_lba_size; i++)
	{
		fout << defalt_data << "\n";
	}
}

void printoperinputerrormsg() {
	std::cout << "잘못된 명령어 입력입니다.\n";
}

void printsuccess() {
	std::cout << "success" << "\n";
}

void printerror() {
	std::cout << "error" << "\n";
}

int main() {
	if (!std::filesystem::exists(file_name)) {
		std::ofstream fout(file_name);
		if (fout.is_open()) {
			init(fout);
			std::cout << "초기화 완료\n";
		}
	}


	try {
		boost::asio::io_context io;

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
				


				std::fstream file(file_name, std::ios::in | std::ios::out);



				std::string cmd(data);
				cmd.erase(cmd.begin()+length, cmd.end());
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
				//std::cin >> op >> lba_idx;
				std::string msg;
				if (op == 'W') {
					std::string value;
					value = cmds[2];
					if (!validlbaidx(lba_idx)) {
						printerror();
						continue;
					}

					if (write(file, lba_idx, value)) {
						printsuccess();
					}
					else {
						printerror();
					}
				}
				else if (op == 'R') {
					if (!validlbaidx(lba_idx)) {
						printerror();
						continue;
					}
					if (read(file, lba_idx,msg)) {
						strcpy(data,msg.c_str());
					}
					else {
						printerror();
					}
				}
				else {
					printoperinputerrormsg();
				}

				file.close();
				
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
