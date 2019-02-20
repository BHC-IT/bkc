#include <bfc/bfc.hpp>

void bfc::masterThread::servProto()
{
	this->_serv.add(301, [](std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string> data){
		std::cout << data.first->first << " says " << data.second << std::endl;
		return (0);
	});
}
