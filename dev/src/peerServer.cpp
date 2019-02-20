#include "peerServer.hpp"
#include "servConnector.hpp"

bkc::node::peerServ::peerServ(blc::tools::pipe &pipe, std::string name, unsigned int maxClient, unsigned int port) : actor(pipe, name), Server(maxClient, port)
{
	this->masterProto();
	this->start();
}

void bkc::node::peerServ::readMaster()
{
}

void bkc::node::peerServ::thick()
{
	struct sockaddr	client;
	int		sock;

	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1));
	while (this->_kill == false){
		if (this->_pipe.readable())
			this->readMaster();
		sock = this->accept(&client);
		if (sock > 0){
			bfc::factory<bkc::node::servCon>("peerServ" + std::to_string(std::rand()), sock, client);
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
	this->close();
}

void bkc::node::peerServ::run(void *userData)
{
	(void)userData;
}