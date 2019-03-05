#include <nlohmann/json.hpp>
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "httpPost.hpp"

using json = nlohmann::json;

static std::string html_res(std::string str)
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
	tmp += "color: green;";
	tmp += "text-align: center;";
	tmp += "}";
	tmp += "p {";
	tmp += "font-family: verdana;";
	tmp += "font-size: 20px;";
	tmp += "}";
	tmp += "</style><h1>bkc status : </h1><br><h2>" + str + "</h2></html>";

	return (tmp);
}

static std::string base_response(std::string str)
{
	std::string res;

	std::string page = html_res(str);

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
	for (auto it : v){
		std::cout << it << std::endl;
	}
	if (v[0].find("GET") != std::string::npos){
		tmp = this->handleGET(v);
		if (tmp == "error")
			this->_client << base_response("error") << blc::endl;
		else
			this->_client << base_response("ok") << blc::endl;
	} else if (v[0].find("POST") != std::string::npos) {
		tmp = this->handlePOST(v);
		this->_client << base_response("ok") << blc::endl;
	} else if (v[0].find("HTTP/1.1") != std::string::npos) {
		this->_client << base_response("ok") << blc::endl;
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
