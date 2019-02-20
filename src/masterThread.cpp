#include <functional>
#include "bfc/masterThread.hpp"

std::map<std::string, bfc::actorRep *>			bfc::masterThread::actors;
std::map<std::string, blc::tools::pipe>			bfc::masterThread::slavePipe;
std::map<std::string, blc::tools::pipe>			bfc::masterThread::masterPipe;
bool							bfc::masterThread::live = true;
int							bfc::masterThread::counter = 0;
std::thread::id						bfc::masterThread::id = std::this_thread::get_id();
blc::tools::pipe					bfc::masterThread::outputPipe;
blc::tools::pipe					bfc::masterThread::outputPipeSlave;
std::string						bfc::masterThread::_myself;

bfc::outStream bfc::cout = bfc::masterThread::outStream();

bfc::masterThread &bfc::masterThread::init()
{
	static bfc::masterThread master;

	return (master);
}

bfc::masterThread::masterThread()
{
	bfc::masterThread::outputPipeSlave = bfc::masterThread::outputPipe.getSlave();

}

bfc::masterThread::~masterThread()
{
	// for (auto it = this->actors.begin(); it != this->actors.end(); it++){
	// 	delete it->second;
	// }
}

void bfc::masterThread::kill()
{
	bfc::masterThread::for_each("*", [](std::map<std::string, blc::tools::pipe>::iterator it){
		std::cout << "killing: " << it->first << std::endl;
		(*bfc::masterThread::actors[it->first]).send(280);
	});
	bfc::masterThread::live = false;
}

void bfc::masterThread::for_each(const std::string &filter, std::function<void(std::map<std::string, blc::tools::pipe>::iterator)> callback)
{
	for (auto it = bfc::masterThread::masterPipe.begin(); it != bfc::masterThread::masterPipe.end(); it++){
		if (blc::tools::regex(filter, it->first)){
			callback(it);
		}
		if (bfc::masterThread::masterPipe.count(it->first) == 0)
			break;
	}
}

void bfc::masterThread::for_each(const std::vector<std::string> &filters, blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>> &protoFact)
{
	for (auto filter : filters){
		bfc::masterThread::for_each(filter, [&protoFact](std::map<std::string, blc::tools::pipe>::iterator it){
			if (it->second.readable()){
				int code = 0;
				try {
					code = std::stoi(it->second.read());
				} catch (std::invalid_argument &e) {
					std::cerr << "error: stoi could not extract code" << blc::endl;
				}
				std::string data = it->second.read();

				protoFact.activate(code, std::make_pair(it, data));
			}
		});
	}
}

int bfc::masterThread::count(const std::string &filter)
{
	int count = 0;

	for (auto it = bfc::masterThread::masterPipe.begin(); it != bfc::masterThread::masterPipe.end(); it++){
		if (blc::tools::regex(filter, it->first)){
			count++;
		}
	}
	return (count);
}

int bfc::masterThread::size()
{
	return (bfc::masterThread::counter);
}

void bfc::masterThread::add(const std::string &name, blc::tools::pipe master, blc::tools::pipe slave)
{
	bfc::masterThread::counter++;
	bfc::masterThread::masterPipe.emplace(name, blc::tools::pipe());
	bfc::masterThread::slavePipe.emplace(name, bfc::masterThread::masterPipe[name].getSlave());
}

void bfc::masterThread::remove(const std::string &name)
{
	bfc::masterThread::counter--;
	bfc::masterThread::masterPipe.erase(name);
	bfc::masterThread::slavePipe.erase(name);
	bfc::masterThread::actors.erase(name);
}

blc::tools::pipe &bfc::masterThread::pipe(const std::string &name)
{
	return (bfc::masterThread::slavePipe[name]);
}

std::thread::id	bfc::masterThread::getId()
{
	return (bfc::masterThread::id);
}

void bfc::masterThread::print(std::string str)
{
	if (std::this_thread::get_id() == bfc::masterThread::getId()) {
		if (str[str.size() - 1] == '\n') {
			str.pop_back();
			std::cout << str << std::endl;
		} else {
			std::cout << str;
		}
	} else {
		bfc::masterThread::outputPipeSlave << str;
	}
}

void bfc::masterThread::print(const char str[])
{
	if (std::this_thread::get_id() == bfc::masterThread::getId()) {
		std::string tmp = std::string(str);
		if (tmp[tmp.size() - 1] == '\n') {
			tmp.pop_back();
			std::cout << tmp << std::endl;
		} else {
			std::cout << tmp;
		}
	} else {
		bfc::masterThread::outputPipeSlave << std::string(str);
	}
}

bfc::actorRep	&bfc::masterThread::actor(const std::string &name)
{
	if (bfc::masterThread::actors.find(name) == bfc::masterThread::actors.end())
		throw blc::error::exception(assertError(std::string("actor \"") + name + "\" doesnt existe"));
	return ((*bfc::masterThread::actors[name]));
}

bfc::actorRep *bfc::masterThread::rep(const std::string &name)
{
	if (bfc::masterThread::actors.find(name) == bfc::masterThread::actors.end())
		throw blc::error::exception(assertError(std::string("actor \"") + name + "\" doesnt existe"));
	return bfc::masterThread::actors[name];
}

bfc::outStream &operator<<(bfc::outStream &stream, bfc::masterThread &master)
{
	return (stream);
}

void bfc::masterThread::lifeCycle()
{
	while (this->live){
		this->thick();
		this->readActor();
		if (bfc::masterThread::outputPipe.readable())
			std::cout << bfc::masterThread::outputPipe.read() << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
	}
}