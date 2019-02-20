#include "bfc/bfc.hpp"
#include "cinConnector.hpp"


bkc::node::cinCon::cinCon(blc::tools::pipe &pipe, std::string name, std::istream &input) : actor(pipe, name), _infd(input)
{
	this->masterProto();
	this->cinProto();

	this->start();
}

static inline void parseInput(std::string &command, std::string &arg)
{
	if (command.find(" ") != std::string::npos){
		arg = command.substr(command.find_first_of(" ") + 1);
		command = command.substr(0, command.find_first_of(" "));
	}
}

void bkc::node::cinCon::readCin()
{
	std::string command;
	std::string arg;

	std::getline(this->_infd, command);
	parseInput(command, arg);
	if (this->_cin.activate(command, arg) == 280){
		this->kill();
	}
}

void bkc::node::cinCon::readMaster()
{
	int code = std::stoi(this->_pipe.read());
	std::string tmp = this->_pipe.read();

	if (this->_masterProto.activate(code, tmp) == 280)
		this->kill();
}

void bkc::node::cinCon::thick()
{
	while (this->isAlive()){
		if (blc::tools::readable(this->_infd)){
			this->readCin();
		} else if (this->_pipe.readable()) {
			this->readMaster();
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
}