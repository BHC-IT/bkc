#include <nlohmann/json.hpp>
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "identity.hpp"
#include "httpPost.hpp"
#include "lists.hpp"

using json = nlohmann::json;

static std::string replace(std::string& s, const std::string& toReplace, const std::string& replaceWith)
{
	std::size_t pos = s.find(toReplace);
	if (pos == std::string::npos) return s;
	return s.replace(pos, toReplace.length(), replaceWith);
}

static std::string replaceAll(std::string& s, const std::string& toReplace, const std::string& replaceWith)
{
	for (; s.find(toReplace) != std::string::npos;){
		replace(s, toReplace, replaceWith);
	}
	return (s);
}

static std::string html_res(std::string str, std::string color)
{
	std::string tmp;

	tmp += "<html>";
	tmp += "<style>";
	tmp += "body {";
	tmp += "background-color: lightblue;";
	tmp += "}";
	tmp += "h1 {";
	tmp += "color: white;";
	tmp += "text-align: center;";
	tmp += "}";
	tmp += "h2 {";
	tmp += "color: " + color + ";";
	tmp += "text-align: center;";
	tmp += "}";
	tmp += "p {";
	tmp += "font-family: verdana;";
	tmp += "font-size: 20px;";
	tmp += "}";
	tmp += "</style><h1>bkc status : </h1><br><h2>" + str + "</h2></html>";

	return (tmp);
}

static std::string base_response(std::string str, std::string color)
{
	std::string res;

	std::string page = html_res(str, color);

	res += "HTTP/1.1 200\r\n";
	res += "status: 200\r\n";
	res += "cache-control: private\r\n";
	res += "connection: close\r\n";
	res += "content-encoding: text/html\r\n";
	res += "content-length: " + std::to_string(page.size()) + "\r\n";
	res += "content-type: text/html\r\n";
	res += "date: Tue, 26 Feb 2019 12:46:33 GMT\r\n";
	res += "server: \r\n";
	res += "vary: Accept-Encoding\r\n\r\n";
	res += page +"\r";
	return (res);
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

std::string bkc::node::servCon::handleGET(const std::vector<std::string> &v) const
{
	int length;
	std::string tmp;
	std::string data = v[0];
	try {
		blc::tools::serializable::cut(data, '?');
		data = blc::tools::serializable::cut(data, ' ');
		data = replaceAll(data, "%22", "\"");
		data = replaceAll(data, "%20", " ");
		std::cout << "data : " << data << std::endl;
		if (data == "")
			return ("");
		json j = json::parse(data);
		tmp = data;
	} catch (std::exception &e) {
		return ("error");
	}
	return (tmp);
}

std::string bkc::node::servCon::handlePOST(const std::vector<std::string> &v) const
{
	int length;
	std::string tmp;

	for (auto it : v){
		if (it.find("content-length:") != std::string::npos){
			blc::tools::serializable::cut(it, ' ');
			length = std::stoi(it);
		}
	}
	tmp = this->_client.read(length);
	return (tmp);
}

void bkc::node::servCon::readPeer()
{
	int				code = 0;
	std::string			tmp = " ";
	std::string			rcv = " ";
	std::vector<std::string>	v;
	bool				http = false;

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
	if (v[0].find("GET") != std::string::npos){
		http = true;
		tmp = this->handleGET(v);
		if (tmp == "error")
			this->_client << base_response("error : bad data format", "red") << blc::endl;
		else
			this->_client << base_response("ok", "green") << blc::endl;
	} else if (v[0].find("POST") != std::string::npos) {
		http = true;
		tmp = this->handlePOST(v);
		this->_client << base_response("ok", "green") << blc::endl;
	} else if (v[0].find("HTTP/1.1") != std::string::npos) {
		http = true;
		this->_client << base_response("ok", "green") << blc::endl;
		return;
	} else {
		if (this->_id_msg == ""){
			this->_id_msg = std::to_string(std::rand());
			json j = {
				{"code", 403},
				{"data", this->_id_msg},
				{"user", bkc::myLog.printablePub()}
			};
			this->_client << j.dump() << blc::endl << blc::endl;
		}
	}
	nlohmann::json js = nlohmann::json::parse(tmp);

	try {
		code = js["code"].get<int>();
		if ((this->_userKey == "" || this->_userKey != js["user"].get<std::string>()) && code > 300 && code < 400 && bkc::lists::isOk(js["user"].get<std::string>())){
			std::cout << "nok" << ", code : " << code << std::endl;
			this->send(280, this->_name);
			this->kill();
			return;
		}
		tmp = js.dump();
		this->_peerProto.activate(code, tmp);
	} catch (std::exception &e) {
		bfc::cout << "error: client code was: " << e.what() << blc::endl;
	}
	if (http){
		this->send(280, this->_name);
		this->kill();
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
