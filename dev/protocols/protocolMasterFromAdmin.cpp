#include <algorithm>
#include <bfc/bfc.hpp>

#include "peerConnector.hpp"
#include "connectedPeer.hpp"
#include "identity.hpp"
#include "chain.hpp"

void bfc::masterThread::adminProto()
{
	this->_adm.add(280, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::kill();
		return (0);
	});
	this->_adm.add(300, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		this->_myself = data.second;
		return (0);
	});
	this->_adm.add(301, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		std::cout << data.first->first << " says " << data.second << std::endl;
		return (0);
	});
	this->_adm.add(302, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		for (int i = data.second.find(';'); i != std::string::npos; i = data.second.find(';')){
			std::string tmp = blc::tools::serializable::cut(data.second, ';');
			if (std::find(this->_knownPeer.begin(), this->_knownPeer.end(), tmp) == this->_knownPeer.end() && tmp !=  bkc::url){
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
	this->_adm.add(303, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		for (auto it = this->_knownPeer.begin(); it != this->_knownPeer.end(); it++){
			if (*it == data.second){
				this->_knownPeer.erase(it);
			}
		}
		return (0);
	});
	this->_adm.add(305, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(305, data.second);
		return (0);
	});
	this->_adm.add(310, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(310, data.second);
		return (0);
	});
	this->_adm.add(320, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(320, data.second);
		return (0);
	});
	this->_adm.add(370, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::actor("chain").send(370, data.second);
		return (0);
	});
	this->_adm.add(470, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		std::string str = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"))->serialize();
		data.first->second << "370" << blc::endl << str << blc::endl;
		return (0);
	});
}
