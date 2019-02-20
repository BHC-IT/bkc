#include <nlohmann/json.hpp>
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "httpPost.hpp"

using json = nlohmann::json;

bkc::node::servCon::servCon(blc::tools::pipe pipe, std::string name, int sock, struct sockaddr client) : actor(pipe, name), _client(sock, client)
{
	this->peerProto();
	this->masterProto();

	this->start();
}

void bkc::node::servCon::readMaster()
{
	int code = 0;
	std::string data = this->_pipe.read();

	try {
		code = std::stoi(data);
	} catch (std::exception &e) {
		bfc::cout << "error: master code was: " << data << blc::endl;
	}
	std::string tmp = this->_pipe.read();

	if (this->_masterProto.activate(code, tmp) == 280){
		this->kill();
	}
}

void bkc::node::servCon::readPeer()
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
		bfc::cout << "error: client code was: " << e.what() << blc::endl;
	}

}

void bkc::node::servCon::serviceLoop(blc::network::Client &cient, blc::network::Client &data)
{
}

void bkc::node::servCon::thick()
{
	while (this->isAlive()){
		if (this->_pipe.readable())
			this->readMaster();
		if (this->_client.readable()){
			this->readPeer();
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
}
