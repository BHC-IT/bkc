#include <nlohmann/json.hpp>
#include "peerConnector.hpp"
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "rsaKey.hpp"
#include "chain.hpp"
#include "identity.hpp"

using json = nlohmann::json;

void bkc::node::peerCon::peerProto()
{
	this->_peerProto.add(101, [=](std::string str){
		std::cout << "connection ok" << std::endl;
		return (0);
	});
	this->_peerProto.add(203, [=](std::string str){
		json j = json::parse(str);
		bkc::rsaKey key;

		key.importPub(j["user"].get<std::string>());
		if (key.verifyPrintable(this->_id_msg, j["data"].get<std::string>())){
			this->_userKey = j["user"].get<std::string>();
			json j = {
				{"code", 101},
				{"data", ""},
				{"user", bkc::myLog.printablePub()}
			};
			this->_client << j.dump() << blc::endl << blc::endl;
		} else {
			std::cout << "failed to connect : bad key" << std::endl;
			json j = {
				{"code", 280},
				{"data", ""},
				{"user", bkc::myLog.printablePub()}
			};

			this->_client << j.dump() << blc::endl << blc::endl;
			this->send(303, this->_id);
			this->send(280, this->getName());
			this->kill();
		}

		return (0);
	});
	this->_peerProto.add(280, [=](std::string str){
		this->send(303, this->_id);
		this->send(280, this->getName());
		this->kill();
		return (280);
	});
	this->_peerProto.add(301, [=](std::string str){
		json j = json::parse(str);

		this->send(301, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(302, [=](std::string str){
		json j = json::parse(str);

		this->send(302, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(305, [=](std::string str){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		json j = json::parse(str);
		bkc::trans t(j["data"].get<std::string>());

		t.setProof(chain->searchProof(t));
		bkc::trans parity = chain->getLeftOver(t);

		parity.setProof(t.getProof());
		j["code"] = 405;
		j["data"] = {
			{"transaction", t.serialize()},
			{"parity", parity.serialize()},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_peerProto.add(310, [=](std::string str){
		json j = json::parse(str);

		this->send(310, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(320, [=](std::string str){
		json j = json::parse(str);

		this->send(320, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(350, [=](std::string str){
		json j = json::parse(str);

		this->send(350, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(352, [=](std::string str){
		json j = json::parse(str);

		this->_id = this->_client.getAddress() + ":" + j["data"].get<std::string>();
		this->send(352, this->_client.getAddress() + j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(370, [=](std::string str){
		json j = json::parse(str);

		this->send(370, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(402, [=](std::string str){
		this->send(402);
		return (0);
	});
	this->_peerProto.add(403, [=](std::string str){
		json j = json::parse(str);
		json res = {
			{"code", 203},
			{"data", bkc::myLog.signPrintable(j["data"].get<std::string>())},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_peerProto.add(470, [=](std::string str){
		this->send(470);
		return (0);
	});
	this->_peerProto.add(490, [=](std::string str){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		json		j = json::parse(str);
		json		res = {
			{"code", 390},
			{"data", chain->getBalance(j["data"].get<std::string>())},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
}

void bkc::node::servCon::peerProto()
{
	this->_peerProto.add(101, [=](std::string str){
		std::cout << "connection ok" << std::endl;
		return (0);
	});
	this->_peerProto.add(203, [=](std::string str){
		json j = json::parse(str);
		bkc::rsaKey key;

		key.importPub(j["user"].get<std::string>());
		if (key.verifyPrintable(this->_id_msg, j["data"].get<std::string>())){
			this->_userKey = j["user"].get<std::string>();
			json j = {
				{"code", 101},
				{"data", ""},
				{"user", bkc::myLog.printablePub()}
			};
			this->_client << j.dump() << blc::endl << blc::endl;
		} else {
			std::cout << "failed to connect : bad key" << std::endl;
			this->send(303, this->_id);
			json j = {
				{"code", 280},
				{"data", ""},
				{"user", bkc::myLog.printablePub()}
			};

			this->_client << j.dump() << blc::endl << blc::endl;
			this->send(280, this->getName());
			this->kill();
		}

		return (0);
	});
	this->_peerProto.add(280, [=](std::string str){
		this->send(303, this->_id);
		this->send(280, this->getName());
		this->kill();
		return (280);
	});
	this->_peerProto.add(301, [=](std::string str){
		json j = json::parse(str);

		this->send(301, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(302, [=](std::string str){
		json j = json::parse(str);

		this->send(302, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(305, [=](std::string str){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		json j = json::parse(str);
		bkc::trans t(j["data"].get<std::string>());

		t.setProof(chain->searchProof(t));
		bkc::trans parity = chain->getLeftOver(t);

		parity.setProof(t.getProof());
		j["code"] = 405;
		j["data"] = {
			{"transaction", t.serialize()},
			{"parity", parity.serialize()},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_peerProto.add(310, [=](std::string str){
		json j = json::parse(str);

		this->send(310, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(320, [=](std::string str){
		json j = json::parse(str);

		this->send(320, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(350, [=](std::string str){
		json j = json::parse(str);

		this->send(350, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(352, [=](std::string str){
		json j = json::parse(str);

		this->_id = this->_client.getAddress() + ":" + j["data"].get<std::string>();
		this->send(352, this->_client.getAddress() + ":" + j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(370, [=](std::string str){
		json j = json::parse(str);

		this->send(370, j["data"].get<std::string>());
		return (0);
	});
	this->_peerProto.add(402, [=](std::string str){
		this->send(402);
		return (0);
	});
	this->_peerProto.add(403, [=](std::string str){
		json j = json::parse(str);
		json res = {
			{"code", 203},
			{"data", bkc::myLog.signPrintable(j["data"].get<std::string>())},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_peerProto.add(470, [=](std::string str){
		this->send(470);
		return (0);
	});
	this->_peerProto.add(490, [=](std::string str){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		json		j = json::parse(str);
		json		res = {
			{"code", 390},
			{"data", chain->getBalance(j["data"].get<std::string>())},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
}