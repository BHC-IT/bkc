#include <nlohmann/json.hpp>
#include <bfc/bfc.hpp>
#include "peerConnector.hpp"
#include "httpPost.hpp"

using json = nlohmann::json;

bkc::node::peerCon::peerCon(blc::tools::pipe pipe, std::string name, std::string addr, int port) : actor(pipe, name), _client(addr, port)
{
	int newPort = 0;

	this->peerProto();
	this->masterProto();

	try {
		this->_client.open();
	} catch (blc::error::exception &e) {
		bfc::cout << assertError(addr + ":" + std::to_string(port)) << blc::endl;
	}

	this->start();
}

void bkc::node::peerCon::readMaster()
{
	int code = 0;
	std::string data = this->_pipe.read();

	try {
		code = std::stoi(data);
	} catch (std::exception &e) {
		bfc::cout << "error: master code was:" << data << blc::endl;
	}
	std::string tmp = this->_pipe.read();

	if (this->_masterProto.activate(code, tmp) == 280){
		this->kill();
	}
}

void bkc::node::peerCon::readPeer()
{
	int				code = 0;
	std::string			tmp = " ";
	std::string			rcv = " ";
	std::vector<std::string>	v;

	while (rcv != "\r" && rcv != ""){
		this->_client >> rcv;
		if (tmp == " ")
			tmp = rcv;
		if (rcv != ""){
			v.push_back(rcv);
		}
	}
	if (bkc::node::isPost(v)){
		bkc::node::httpPost test(v);

		tmp = this->_client.read(test.getLength());
		v.push_back(tmp);
		this->_client << "HTTP/1.1 200 OK\r\nDate: Sat, 03 Dec 2005 11:25:07 GMT\r\nServer: Apache/2.0.52 (Fedora)\r\nLast-Modified: Sat, 03 Dec 2005 10:23:07 GMT\r\nETag: \"2b8b2-150-857bb381\"\r\nAccept-Ranges: bytes\r\nContent-Length: 416\r\nKeep-Alive: timeout=10, max=100\r\nConnection: close\r\nContent-Type: text/html; charset=ISO-8859-1\r\nContent-Length: 2\r\n\r\nok\r\n";
	}
	nlohmann::json js = nlohmann::json::parse(tmp);

	try {
		code = js["code"].get<int>();
		tmp = js.dump();
		this->_peerProto.activate(code, tmp);
	} catch (std::exception &e) {
		bfc::cout << "error: client code was:" << e.what() << blc::endl;
	}
}

void bkc::node::peerCon::thick()
{
	this->_pipe = bfc::masterThread::pipe(this->getName());
	// this->_client << "{\"data\":\"okidoki\", \"code\": 301}" << blc::endl << blc::endl;
	while (this->isAlive()){
		if (this->_pipe.readable())
			this->readMaster();
		else if (this->_client.readable()) {
			this->readPeer();
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
	this->_client.close();
}
