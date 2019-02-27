#include <nlohmann/json.hpp>
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "httpPost.hpp"

using json = nlohmann::json;

static std::string base_response()
{
	std::string str;

	str += "HTTP/1.1 200\r\n";
	str += "status: 200\r\n";
	str += "cache-control: private\r\n";
	str += "connection: close\r\n";
	str += "content-encoding: text/html\r\n";
	str += "content-length: 6\r\n";
	str += "content-type: text/html\r\n";
	str += "date: Tue, 26 Feb 2019 12:46:33 GMT\r\n";
	str += "server: \r\n";
	str += "vary: Accept-Encoding\r\n\r\n";
	str += "bkc ok\r";
	return (str);
}

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
	if (v.size() == 0 || v[0].size() == 0)
		return;
	if (v[0].find("POST") != std::string::npos) {
		int length;

		for (auto it : v){
			if (it.find("content-length:") != std::string::npos){
				blc::tools::serializable::cut(it, ' ');
				length = std::stoi(it);
			}
		}
		tmp = this->_client.read(length);
		this->_client << base_response() << blc::endl;
	} else if (v[0].find("HTTP/1.1") != std::string::npos) {
		this->_client << base_response() << blc::endl;
		return;
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
			try {
				this->readPeer();
			} catch (nlohmann::detail::parse_error &e) {
				std::cerr << "error parsing: " << e.what() << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
}
