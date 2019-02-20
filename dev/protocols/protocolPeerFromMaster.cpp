#include <nlohmann/json.hpp>
#include "peerConnector.hpp"
#include "servConnector.hpp"
#include "identity.hpp"


using json = nlohmann::json;

void bkc::node::peerCon::masterProto()
{
	this->_masterProto.add(280, [=](std::string str){
		json j = {
			{"code", 280},
			{"data", ""},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		this->kill();
		bfc::masterThread::remove(str);
		return (0);
	});
	this->_masterProto.add(301, [=](std::string str){
		json j = {
			{"code", 301},
			{"data", str},
			{"user", bkc::myLog.printablePub()},
			{"sign", bkc::myLog.signPrintable(str)}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(302, [=](std::string str){
		json j = {
			{"code", 302},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(305, [=](std::string str){
		json j = {
			{"code", 305},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(310, [=](std::string str){
		json j = {
			{"code", 310},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(320, [=](std::string str){
		json j = {
			{"code", 320},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(350, [=](std::string str){
		json j = {
			{"code", 350},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(352, [=](std::string str){
		json j = {
			{"code", 352},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(370, [=](std::string str){
		json j = {
			{"code", 370},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(401, [=](std::string str){
		json j = {
			{"code", 401},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(470, [=](std::string str){
		json j = {
			{"code", 470},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
}

void bkc::node::servCon::masterProto()
{
	this->_masterProto.add(280, [=](std::string str){
		json j = {
			{"code", 280},
			{"data", ""},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		this->kill();
		bfc::masterThread::remove(str);
		return (0);
	});
	this->_masterProto.add(301, [=](std::string str){
		json j = {
			{"code", 301},
			{"data", str},
			{"user", bkc::myLog.printablePub()},
			{"sign", bkc::myLog.signPrintable(str)}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(302, [=](std::string str){
		json j = {
			{"code", 302},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(305, [=](std::string str){
		json j = {
			{"code", 305},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(310, [=](std::string str){
		json j = {
			{"code", 310},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(320, [=](std::string str){
		json j = {
			{"code", 320},
			{"data", str},
			{"user", bkc::myLog.printablePub()}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(350, [=](std::string str){
		json j = {
			{"code", 350},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(352, [=](std::string str){
		json j = {
			{"code", 352},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(370, [=](std::string str){
		json j = {
			{"code", 370},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(401, [=](std::string str){
		json j = {
			{"code", 401},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
	this->_masterProto.add(470, [=](std::string str){
		json j = {
			{"code", 470},
			{"data", str},
			{"user", "lanath"}
		};

		this->_client << j.dump() << blc::endl << blc::endl;
		return (0);
	});
}
