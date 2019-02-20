#pragma once

#include <blc/blc.hpp>
#include <bfc/bfc.hpp>

namespace bkc {
	namespace node {
		class peerServ : public blc::network::Server, public bfc::actor<peerServ> {
		public:
			peerServ(blc::tools::pipe &pipe, std::string name, unsigned int maxClient, unsigned int port);
			~peerServ() = default;

			void	masterProto();

			void	readMaster();

			void	thick();
			void	run(void *userData);
		private:
		};
	}
}