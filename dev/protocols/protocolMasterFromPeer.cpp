#include <algorithm>
#include <bfc/bfc.hpp>

#include "peerConnector.hpp"
#include "connectedPeer.hpp"
#include "chain.hpp"
#include "lists.hpp"

void bfc::masterThread::peerProto()
{
	this->_peer.add(280, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::remove(data.second);
		return (0);
	});
	this->_peer.add(301, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		std::cout << data.first->first << " says " << data.second << std::endl;
		return (0);
	});
	this->_peer.add(302, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		for (int i = data.second.find(';'); i != std::string::npos; i = data.second.find(';')){
			std::string tmp = blc::tools::serializable::cut(data.second, ';');
			if (std::find(this->_knownPeer.begin(), this->_knownPeer.end(), tmp) == this->_knownPeer.end() && tmp != bfc::masterThread::_myself){
				int port = 0;
				std::string addr = tmp.substr(0, tmp.find(':'));

				try {
					port = std::stoi(tmp.substr(tmp.find(':') + 1, tmp.size()));
				} catch (blc::error::exception &e) {
					bfc::cout << tmp.substr(tmp.find(':') + 1, tmp.size()) << blc::endl;
					throw blc::error::exception("bad number");
				}

				this->_knownPeer.push_back(tmp);
				if (bkc::connectedPeer::isConnected(addr, port) == true) {
					std::cout << tmp << " is duble" << std::endl;
				} else {
					bfc::factory<bkc::node::peerCon>("peerClient" + std::to_string(std::rand()), addr, port);
					bkc::connectedPeer::connect(addr, port);
				}
				bfc::cout << "new peer : " << tmp << blc::endl;
			}
		}
		return (0);
	});
	this->_peer.add(303, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		const bkc::connectedPeer	&con = bkc::connectedPeer::init();

		con.erase(data.second);
		return (0);
	});
	this->_peer.add(305, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(305, data.second);
		return (0);
	});
	this->_peer.add(310, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(310, data.second);
		return (0);
	});
	this->_peer.add(320, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(320, data.second);
		return (0);
	});
	this->_peer.add(350, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::cout << "username : " << data.second << blc::endl;

		return (0);
	});
	this->_peer.add(352, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bkc::connectedPeer::connect(data.second);

		return (0);
	});
	this->_peer.add(370, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(370, data.second);
		return (0);
	});
	this->_peer.add(371, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		json j = json::parse(data.second);
		bool white = false;
		bool black = false;

		if (j.count("whitelist"))
			white = j["whitelist"].get<bool>();
		if (j.count("blacklist"))
			black = j["blacklist"].get<bool>();
		bkc::lists::unserialize(j["list"].get<std::string>(), j["whitelist"].get<bool>(), j["blacklist"].get<bool>());
		return (0);
	});
	this->_peer.add(402, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		const bkc::connectedPeer	&con = bkc::connectedPeer::init();

		data.first->second << "302" << blc::endl << con.serialize() << blc::endl;
		return (0);
	});
	this->_peer.add(470, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		std::string str = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"))->serialize();
		data.first->second << "370" << blc::endl << str << blc::endl;
		return (0);
	});
}
