#include <blc/serializable.hpp>
#include <blc/errorHandle.hpp>
#include "bfc/actorRep.hpp"
#include "bfc/masterThread.hpp"

bfc::actorRep::actorRep(blc::tools::pipe pipe, std::string name) : _pipe(pipe), _live(true), _name(name)
{
}

void bfc::actorRep::kill()
{
	this->_live = false;
}

bool bfc::actorRep::isAlive() const
{
	return (this->_live);
}

void bfc::actorRep::send(int code, std::string message)
{
	if (this->getId() != std::this_thread::get_id()){
		bfc::masterThread::for_each(this->getName(), [=](std::map<std::string, blc::tools::pipe>::iterator it){
			it->second << std::to_string(code) << blc::endl << message << blc::endl;
		});
	} else {
		this->_pipe << std::to_string(code) << blc::endl << message << blc::endl;
	}
}

std::string bfc::actorRep::getName() const
{
	return (this->_name);
}

std::thread::id	bfc::actorRep::getId() const
{
	return (this->_id);
}

void bfc::actorRep::setId(std::thread::id id)
{
	this->_id = id;
}