#include <nlohmann/json.hpp>
#include "adminConnector.hpp"
#include "rsaKey.hpp"
#include "identity.hpp"
#include "chain.hpp"

using json = nlohmann::json;

void bkc::node::admCon::admProto()
{
	this->_admProto.add(101, [=](std::string str){
		std::cout << "connection ok" << std::endl;
		json j = {
			{"code", 352},
			{"data", std::to_string(bkc::port)},
			{"user", bkc::myLog.printablePub()}
		};
		this->_client << j.dump() << blc::endl << blc::endl;
		j["code"] = 402;
		j["data"] = "";
		this->_client << j.dump() << blc::endl << blc::endl;
		j["code"] = 470;
		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_admProto.add(203, [=](std::string str){
		json j = json::parse(str);
		bkc::rsaKey key;

		key.importPub(j["user"].get<std::string>());
		if (key.verifyPrintable(this->_id_msg, j["data"].get<std::string>()) && bkc::admKey.printablePub() == j["user"].get<std::string>()){
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
			this->send(280, this->getName());
			this->kill();
		}
		return (0);
	});
	this->_admProto.add(280, [=](std::string str){
		this->send(280, this->getName());
		this->kill();
		return (280);
	});
	this->_admProto.add(300, [=](std::string str){
		json j = json::parse(str);

		this->send(300, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(301, [=](std::string str){
		json j = json::parse(str);
		bkc::rsaKey key;

		try {
			key.importPub(j["user"].get<std::string>());
			if (key.verifyPrintable(j["data"].get<std::string>(), j["sign"].get<std::string>()))
				this->send(301, j["data"].get<std::string>());
		} catch (json::type_error &e) {
			bfc::cout << this->getName() << ": data badly formated" << blc::endl;
		}
		return (0);
	});
	this->_admProto.add(302, [=](std::string str){
		json j = json::parse(str);

		this->send(302, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(305, [=](std::string str){
		json j = json::parse(str);

		this->send(305, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(310, [=](std::string str){
		json j = json::parse(str);

		this->send(310, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(320, [=](std::string str){
		json j = json::parse(str);

		this->send(320, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(350, [=](std::string str){
		json j = json::parse(str);

		this->send(350, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(352, [=](std::string str){
		json j = json::parse(str);

		this->send(352, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(370, [=](std::string str){
		json j = json::parse(str);

		this->send(370, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(401, [=](std::string str){
		json j = json::parse(str);

		this->send(401, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(403, [=](std::string str){
		json j = json::parse(str);
		json res = {
			{"code", 203},
			{"data", bkc::myLog.signPrintable(j["data"].get<std::string>())},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << res.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_admProto.add(470, [=](std::string str){
		json j = json::parse(str);

		this->send(470, j["data"].get<std::string>());
		return (0);
	});
	this->_admProto.add(490, [=](std::string str){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		json		j = json::parse(str);

		this->_client << "390" << blc::endl << std::to_string(chain->getBalance(j["data"].get<std::string>())) << blc::endl;
		return (0);
	});
}
