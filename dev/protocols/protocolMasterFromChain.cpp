#include <bfc/bfc.hpp>
#include "chain.hpp"
#include "identity.hpp"

void bfc::masterThread::chainProto()
{
	this->_chain.add(280, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){

		return (0);
	});
}
