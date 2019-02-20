#pragma once

#include <blc/blc.hpp>
#include "bfc/actorRep.hpp"

namespace bfc {
	template <typename T>
	class actor : public actorRep, public blc::tools::actor<T> {
	public:
		actor(blc::tools::pipe pipe, std::string name) : actorRep(pipe, name){};

		virtual	void	masterProto() = 0;
		virtual void	thick() = 0;
		virtual void	lifeCycle(){
			this->setId(std::this_thread::get_id());
			while (this->isAlive()){
				this->thick();
				std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5));
			}
		};
	protected:
		blc::tools::protocolFactory<int, std::string>	_masterProto;
	private:
	};
}