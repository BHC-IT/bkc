#include "connectedPeer.hpp"

std::vector<std::string>	bkc::connectedPeer::_con;
std::mutex			bkc::connectedPeer::_access;

bkc::connectedPeer &bkc::connectedPeer::init()
{
	static bkc::connectedPeer peer;

	return (peer);
}


std::vector<std::string> bkc::connectedPeer::getConnected()
{
	return (bkc::connectedPeer::_con);
}

bool bkc::connectedPeer::isConnected(const std::string &str)
{
	bkc::connectedPeer::_access.lock();
	for (auto it : bkc::connectedPeer::_con){
		if (it == str){
			std::cout << "test : " << it << "|" << str << std::endl;
			return (true);
		}
	}
	bkc::connectedPeer::_access.unlock();
	return (false);
}

bool bkc::connectedPeer::isConnected(const std::string &str, int port)
{
	return (bkc::connectedPeer::isConnected(str + std::to_string(port)));
}

bool bkc::connectedPeer::connect(const std::string &str)
{
	if (isConnected(str)){
		return (false);
	}
	bkc::connectedPeer::_access.lock();
	bkc::connectedPeer::_con.push_back(str);
	bkc::connectedPeer::_access.unlock();
	return (true);
}

bool bkc::connectedPeer::connect(const std::string &str, int port)
{
	return (bkc::connectedPeer::connect(str + std::to_string(port)));
}

void bkc::connectedPeer::erase(const std::string &str)
{
	bkc::connectedPeer::_access.lock();
	auto tmp = bkc::connectedPeer::_con.end();

	for (auto it = bkc::connectedPeer::_con.begin(); it != bkc::connectedPeer::_con.end(); it++){
		if (*it == str)
			tmp = it;
	}
	if (tmp != bkc::connectedPeer::_con.end())
		bkc::connectedPeer::_con.erase(tmp);
	bkc::connectedPeer::_access.unlock();
}

void bkc::connectedPeer::erase(const std::string &str, int port)
{
	bkc::connectedPeer::erase(str + std::to_string(port));
}

unsigned int bkc::connectedPeer::size()
{
	return (bkc::connectedPeer::_con.size());
}

std::string bkc::connectedPeer::serialize() const
{
	std::string seri;

	for (auto it : bkc::connectedPeer::_con){
		seri += it;
		seri += ";";
	}
	return (seri);
}

void bkc::connectedPeer::unserialize(const std::string &str)
{
	std::string data = str;

	for (int i = data.find(';'); i != std::string::npos; i = data.find(';')){
		std::string tmp = blc::tools::serializable::cut(data, ';');
		bkc::connectedPeer::connect(tmp);
	}
}

