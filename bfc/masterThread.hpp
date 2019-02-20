#pragma once

#include <map>
#include <blc/blc.hpp>
#include "bfc/actorRep.hpp"

namespace bfc {
	class masterThread : public blc::tools::nonCopyable {
	public:
		~masterThread();
		static masterThread	&init();

		static void		kill();
		static void		for_each(const std::string &filter, std::function<void(std::map<std::string, blc::tools::pipe>::iterator)> it);
		static void		for_each(const std::vector<std::string> &filters, blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>> &protoFact);
		static int		count(const std::string &filter);
		static int		size();
		static void		add(const std::string &name, blc::tools::pipe master, blc::tools::pipe slave);
		static void		remove(const std::string &name);
		static blc::tools::pipe	&pipe(const std::string &name);
		static std::thread::id	getId();
		static void		print(std::string str);
		static void		print(const char str[]);
		template <typename T>
		static void		print(T &&nb);
		static bfc::actorRep	&actor(const std::string &name);
		static bfc::actorRep	*rep(const std::string &name);

		void lifeCycle();

		static std::map<std::string, bfc::actorRep *>		actors;
		static std::map<std::string, blc::tools::pipe>		masterPipe;
		static std::string					_myself;

		class outStream {
		public:
			outStream &operator<<(const std::string &str) {print(str); return (*this);};
			outStream &operator<<(const char str[]) {print(str); return (*this);};
			outStream &operator<<(int nb) {print(nb); return (*this);};
			outStream &operator<<(float nb) {print(nb); return (*this);};
			outStream &operator<<(double nb) {print(nb); return (*this);};
		};

		void adminProto();
		void cinProto();
		void peerProto();
		void servProto();
		void chainProto();

	private:
		void thick();
		masterThread();

		std::vector<std::string>				_knownPeer;

		static blc::tools::pipe					outputPipeSlave;
		static blc::tools::pipe					outputPipe;
		static std::map<std::string, blc::tools::pipe>		slavePipe;
		static bool						live;
		static int						counter;
		static std::thread::id					id;

		blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>>	_adm;
		blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>>	_cin;
		blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>>	_serv;
		blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>>	_peer;
		blc::tools::protocolFactory<int, std::pair<std::map<std::string, blc::tools::pipe>::iterator, std::string>>	_chain;


		void readActor();
	};

	using	outStream = masterThread::outStream;
	extern 	masterThread::outStream cout;
}

template <typename T>
void bfc::masterThread::print(T &&nb)
{
	if (std::this_thread::get_id() == bfc::masterThread::getId())
		std::cout << nb;
	else
		bfc::masterThread::outputPipeSlave << std::to_string(nb);
}


bfc::outStream &operator<<(bfc::outStream &stream, bfc::masterThread &master);
