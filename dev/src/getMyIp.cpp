#include <blc/blc.hpp>
#include "getMyIp.hpp"

std::string bkc::getMyIp()
{
	// blc::network::Socket		sock("bot.whatismyipaddress.com", 80);
	// std::string			tmp = " ";
	// std::vector<std::string>	v;
	// std::string			rcv = " ";

	// try {
	// 	sock.open();
	// } catch (blc::error::exception &e) {
	// 	std::cerr << assertError(e.what()) << std::endl;
	// 	throw blc::error::exception(assertError(e.what()));
	// }

	// sock << "GET / HTTP/1.1\r" << blc::endl;
	// sock << "host: bot.whatismyipaddress.com\r" << blc::endl;
	// sock << "accept: */*\r" << blc::endl;
	// sock << "accept-encoding:\r" << blc::endl;
	// sock << "cache-control: no-cache\r" << blc::endl;
	// sock << "postman-token: eb0c0fdf-8162-446f-b5ad-c93bed4884d5\r" << blc::endl;
	// sock << "user-agent: PostmanRuntime/7.3.0" << blc::endl << "\r" << blc::endl;

	// while (sock.readable() == false);
	// while (rcv != "\r" && rcv != ""){
	// 	sock >> rcv;
	// 	if (tmp == " ")
	// 		tmp = rcv;
	// 	if (rcv != ""){
	// 		v.push_back(rcv);
	// 	}
	// }

	// std::string length;
	// for (auto it : v){
	// 	std::string data = it;
	// 	if (blc::tools::serializable::cut(data, ' ') == "Content-Length:")
	// 		length = data;
	// }
	// try {
	// 	tmp = sock.read(std::stoi(length) * 2);
	// } catch (std::invalid_argument &e){
	// 	throw blc::error::exception(assertError(e.what()));
	// }
	return ("127.0.0.1");
}