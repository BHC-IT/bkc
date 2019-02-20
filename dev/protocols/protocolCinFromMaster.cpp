#include "cinConnector.hpp"

void bkc::node::cinCon::masterProto()
{
	this->_masterProto.add(280, [=](std::string str){
		bfc::masterThread::remove(str);
		return (280);
	});
}
