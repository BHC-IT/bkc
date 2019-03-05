#pragma once

#include <blc/blc.hpp>

namespace bfc {
	class actorRep : public blc::symbole::nonCopyable {
	public:
		actorRep(blc::tools::pipe pipe, std::string name);
		virtual ~actorRep() = default;

		void		kill();
		bool		isAlive() const;
		void		send(int code, std::string message = "");
		std::string 	getName() const;
		std::thread::id	getId() const;
		void		setId(std::thread::id id);

	protected :
		blc::tools::pipe	_pipe;
		bool			_live;
		std::string		_name;
		std::thread::id		_id;
	private:
	};
}