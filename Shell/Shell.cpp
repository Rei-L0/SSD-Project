#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/asio.hpp>
#include <fstream>
#include <filesystem>
#include "FileSystem.h"

using boost::asio::ip::tcp;

class MyTeam {
private:
	std::string team_name = "민코딩";
	std::string teamone1_name = "심동현";
	std::string teamone2_name = "박연후";
	std::string myeonglyeong = R"(
	각 명령어 사용법
	알아서 잘 쓰세요
)";

public:
	void MyTeam_explaine() {
		std::cout << "팀이름 : " << team_name << std::endl;
		std::cout << "팀 원 : " << teamone1_name << ", " << teamone2_name << std::endl;
		std::cout << "팀이름 : " << myeonglyeong << std::endl;
	}

};

void Error_Out(FS& Fs) {
	std::string msg;
	msg = "ERROR";
	Fs.write(msg);
	std::cout << msg << std::endl;
}

void CMD_Error() {
	std::cout << "명령어 인자 갯수 에러" << std::endl;
}

int Check_LBA(std::string cmd) {
	if (cmd.size() > 2)return -1;
	if (cmd[0] < '0' or cmd[0]>'9') return -1;
	if (cmd.size() == 2) {
		if (cmd[1] < '0' or cmd[1]>'9') return -1;
		if (cmd[0] == '0')return-1;
	}
	return std::stoi(cmd);
}

int Check_Value(std::string cmd) {
	if (cmd.size() != 10) {
		std::cout << "밸류 값 자릿수 에러"<<std::endl;
		return 0;
	}
		
	if(cmd[0] != '0' or cmd[1] != 'x') {
		std::cout << "Value 자료형 표현 에러"<<std::endl;
		return 0;
	}
	for (int i = 2; i < cmd.size(); i++) {
		if (!((cmd[i] >= '0' and cmd[i] <= '9') or (cmd[i] >= 'a' and cmd[i] <= 'f'))) {
			std::cout << "Value 값 범위 에러"; return 0;
		}
	}
	return 1;
}

std::string Tcp_send(std::string msg, tcp::socket& socket) {
	boost::asio::write(socket, boost::asio::buffer(msg));

	char reply[1024];
	size_t length = socket.read_some(boost::asio::buffer(reply));

	return  std::string(reply, length);
}

int main() {
	FS Fs;
	MyTeam team4;

	try {
		boost::asio::io_context io;

		tcp::socket socket(io);
		socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345));

		std::cout << "Connected to server\n";

		while (true) {
			std::string shell = "team4>";
			Fs.write(shell);
			std::cout << shell;

			std::string msg;

			std::string cmd;
			std::getline(std::cin, cmd);

			Fs.write(cmd);

			std::stringstream ss(cmd);
			std::vector<std::string> cmds;
			std::string tem;

			while (std::getline(ss, tem, ' ')) {
				if (tem != "") {
					cmds.push_back(tem);
				}
			}

			


			if (cmds[0] == "write") {
				if (cmds.size() != 3) {
					Error_Out(Fs);
					//CMD_Error();
					continue;
				}

				int LBA = Check_LBA(cmds[1]);
				if (LBA < 0 or 99 < LBA) {
					Error_Out(Fs);
					continue;
				}

				if (!Check_Value(cmds[2])) {
					Error_Out(Fs);
					continue;
				}

				msg = "W " + cmds[1];
				msg += " ";
				msg += cmds[2];
				std::string reply = Tcp_send(msg, socket);
				std::cout << reply << std::endl;
				Fs.write(reply);
				std::cout << "SUCCESS" << std::endl;
			}
			else if (cmds[0] == "read") {
				if (cmds.size() != 2) {
					Error_Out(Fs);
					//CMD_Error();
					continue;
				}

				int LBA = Check_LBA(cmds[1]);
				if (LBA < 0 or 99 < LBA) {
					Error_Out(Fs);
					continue;
				}

				msg = "R " + cmds[1];
				std::string reply = Tcp_send(msg, socket);
				reply = "0x" + reply;
				Fs.write(reply);
				std::cout << reply << std::endl;
				std::cout << "SUCCESS" << std::endl;
			}
			else if (cmds[0] == "exit")		return 0;
			else if (cmds[0] == "help") 	team4.MyTeam_explaine();
			else if (cmds[0] == "fullwrite") {
				if (cmds.size() != 2) {
					Error_Out(Fs);
					//CMD_Error();
					continue;
				}

				if (!Check_Value(cmds[1])) {
					Error_Out(Fs);
					continue;
				}

				std::vector<std::string> fullwrite_tmp;
				msg = "R ";

				//read
				for (int i = 0; i < 100; i++) {
					Tcp_send(msg + std::to_string(i), socket);
				}

				msg = "W ";
				//write
				std::string reply;
				for (int i = 0; i < 100; i++) {
					reply = Tcp_send(msg + std::to_string(i) + " " + cmds[1], socket);
					if (reply == "ERROR") {
						for (int j = 0; j < i; i++) {
							Tcp_send(msg + std::to_string(j) + " " + cmds[1], socket);
						}
						break;
					}
				}
				Fs.write(reply);
				std::cout << reply << std::endl;
			}
			else if (cmds[0] == "fullread") {
				if (cmds.size() != 1) {
					Error_Out(Fs);
					continue;
				}

				msg = "R ";
				std::string reply;
				for (int i = 0; i < 100; i++) {
					reply = Tcp_send(msg + std::to_string(i), socket);
					if (reply == "ERROR") {
						Fs.write(reply);
						std::cout << reply << std::endl;
						break;
					}
					else {
						reply = "LBA " + std::to_string(i) + " : 0x" + reply;
						Fs.write(reply);
						std::cout << reply << std::endl;
					}
				}
			}
			else if (cmds[0] == "test") {
				if (cmds.size() != 2) {
					Error_Out(Fs);
					continue;
				}



			}
			else if (cmds[0] == "testall") 	return 0;
			else {
				Error_Out(Fs);
			}
		}


	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}


	


	return 0;
}
