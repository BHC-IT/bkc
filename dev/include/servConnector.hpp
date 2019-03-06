#pragma once

#include <blc/blc.hpp>
#include <bfc/bfc.hpp>

namespace bkc {
	namespace node {
		class servCon : public bfc::actor<servCon> {
		public:
			servCon() = delete;
			servCon(blc::tools::pipe pipe, std::string name, int sock, struct sockaddr client);

			void peerProto();
			void masterProto();

			std::string handleGET(const std::vector<std::string> &v) const;
			std::string handlePOST(const std::vector<std::string> &v) const;

			void readMaster();
			void readPeer();

			void serviceLoop(blc::network::Client &cient, blc::network::Client &data);
			void thick();
		private:
			blc::network::Client				_client;
			blc::tools::protocolFactory<int, std::string>	_peerProto;
			std::string					_id;
			std::string					_userKey;
			std::string					_id_msg;
		};
	}
}