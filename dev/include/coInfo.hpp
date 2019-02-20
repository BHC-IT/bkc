#pragma once

#include <string>

namespace bkc {
	namespace node {
		typedef struct	co_info_s
		{
			std::string	addr;
			uint		port;
			uint		max_co;
		}		co_info_t;
	}
}