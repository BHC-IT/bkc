#pragma once

#include "pipeHolder.hpp"
#include "protocolPeerToPeer.hpp"
#include "coInfo.hpp"
#include "chainInfo.hpp"

namespace bkc {
	namespace node {
		class meta : public pipeHolder {
		public:
			meta();
		private:
			chain_info_s	_chain;
			co_info_t	_connector;
		}
	}
}