#include <nlohmann/json.hpp>
#include "peerConnector.hpp"
#include "servConnector.hpp"
#include "connectedPeer.hpp"
#include "rsaKey.hpp"
#include "chain.hpp"

using json = nlohmann::json;

void bkc::node::peerCon::peerProto()
{
	this->_peerProto.add(280, [=](std::string str){
		this->kill();
		this->send(303, this->_id);
		this->send(280, this->getName());
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
			{"parity", parity.serialize()}
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
	this->_peerProto.add(401, [=](std::string str){
		this->send(401);
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
			{"data", chain->getBalance(j["data"].get<std::string>())}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
}

void bkc::node::servCon::peerProto()
{
	this->_peerProto.add(280, [=](std::string str){
		this->kill();
		this->send(303, this->_id);
		this->send(280, this->getName());
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
			{"parity", parity.serialize()}
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
	this->_peerProto.add(401, [=](std::string str){
		this->send(401);
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
			{"data", chain->getBalance(j["data"].get<std::string>())}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
}