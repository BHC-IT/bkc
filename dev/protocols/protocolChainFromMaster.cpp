#include "chain.hpp"

void bkc::chain::masterProto()
{
	this->_masterProto.add(280, [=](std::string str){
		bfc::masterThread::remove(str);
		return (280);
	});
	this->_masterProto.add(310, [=](std::string str){
		json j = json::parse(str);
		bkc::trans t(j["transaction"].get<std::string>());
		bkc::trans parity(j["parity"].get<std::string>());

		std::cout << this->verify(t) << "|" << this->verify(parity) << "|" << this->leftOver(t, parity) << std::endl;
		if (this->verify(t) == true && this->verify(parity) && this->leftOver(t, parity) == 0){
			this->_book.add(t);
			this->_book.add(parity);
		}
		return (1);
	});
	this->_masterProto.add(320, [=](std::string str){
		json		j = json::parse(str);
		bkc::trans	t(j["creation"].get<std::string>());

		if (this->_admKeyStr == t.getSender() && this->_admKeyStr == t.getReceiver() && t.getProof() == ""){
			std::cout << t.getAmount() << " created" << std::endl;
			this->_book.add(t);
		}
		return (1);
	});
	this->_masterProto.add(370, [=](std::string str){
		this->unserialize(str); // need to be verified
		return (0);
	});
}
