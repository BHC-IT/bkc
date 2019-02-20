#pragma once

#include <type_traits>
#include <blc/blc.hpp>
#include "bfc/masterThread.hpp"

namespace bfc {
	template<class func>
	class factory {
	public:
		template<typename... Args>
		factory(std::string name, Args&&... args)
		{
			if (std::is_base_of<bfc::actor<func>, func>::value == false)
				throw blc::error::exception("the type given does not inherit from actor");

			blc::tools::pipe tmp;
			bfc::masterThread::add(name, tmp, tmp.getSlave());

			bfc::masterThread::actors[name] = new func(bfc::masterThread::pipe(name), name, std::forward<Args>(args)...);
		}
	};
}