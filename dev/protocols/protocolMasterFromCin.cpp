#include <bfc/bfc.hpp>
#include "chain.hpp"
#include "identity.hpp"

void bfc::masterThread::cinProto()
{
	this->_cin.add(280, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bfc::masterThread::kill();
		return (0);
	});
	this->_cin.add(301, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		if (bfc::flags::isSet("a") == false)
			bfc::masterThread::actor("adm").send(301, data.second);
		bfc::masterThread::for_each("peer*", [=](std::map<std::string, blc::tools::pipe>::iterator it){
			bfc::masterThread::actor(it->first).send(301, data.second);
		});
		return (0);
	});
	this->_cin.add(305, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bkc::trans t = bkc::trans::createTrans(bkc::myLog.printablePub(), bkc::myLog.printablePub(), std::stod(data.second), bkc::myLog);

		if (bfc::flags::isSet("a") == false)
			bfc::masterThread::actor("adm").send(305, t.serialize());
		bfc::masterThread::for_each("peer*", [=](std::map<std::string, blc::tools::pipe>::iterator it){
			bfc::masterThread::actor(it->first).send(305, t.serialize());
		});
		bfc::masterThread::actor("chain").send(305, t.serialize());
		return (0);
	});
	this->_cin.add(310, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));
		bkc::trans 	t = bkc::trans::createTrans(bkc::myLog.printablePub(), bkc::myLog.printablePub(), std::stod(data.second), bkc::myLog);

		t.setProof(chain->searchProof(t));
		if (t.getProof() != ""){
			bkc::trans parity = chain->getLeftOver(t);
			parity.setProof(t.getProof());
			json j = {
				{"transaction", t.serialize()},
				{"parity", parity.serialize()},
			};

			if (bfc::flags::isSet("a") == false)
				bfc::masterThread::actor("adm").send(310, j.dump());
			bfc::masterThread::for_each("peer*", [=](std::map<std::string, blc::tools::pipe>::iterator it){
				bfc::masterThread::actor(it->first).send(310, j.dump());
			});
			bfc::masterThread::actor("chain").send(310, j.dump());
			return (0);
		}
		std::cout << "not enougth funds : " << data.second << std::endl;
		return (-1);
	});
	this->_cin.add(320, [=](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bkc::trans 	t = bkc::trans::createTrans(bkc::myLog.printablePub(), bkc::myLog.printablePub(), std::stod(data.second), bkc::myLog);

		if (bfc::flags::isSet("a")){
			json j = {
				{"creation", t.serialize()},
			};

			bfc::masterThread::for_each("peer*", [=](std::map<std::string, blc::tools::pipe>::iterator it){
				bfc::masterThread::actor(it->first).send(320, j.dump());
			});
			bfc::masterThread::actor("chain").send(320, j.dump());
			return (0);
		}
		std::cout << "you are not an admin" << std::endl;
		return (-1);
	});
	this->_cin.add(420, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		for (auto it = this->_knownPeer.begin(); it != this->_knownPeer.end(); it++){
			std::cout << *it << std::endl;
		}
		return (0);
	});
	this->_cin.add(490, [this](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));

		std::cout << "balance: " << chain->getBalance(bkc::myLog.printablePub()) << std::endl;
		return (0);
	});
}
