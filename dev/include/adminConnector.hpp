#pragma once

#include <blc/blc.hpp>
#include <bfc/bfc.hpp>

namespace bkc {
	namespace node {
		class admCon : public bfc::actor<admCon> {
		public:
			admCon() = delete;
			admCon(blc::tools::pipe pip, std::string name, std::string addr, int port);

			void admProto();
			void masterProto();

			void readMaster();
			void readAdm();

			void thick();
		private:
			blc::network::Socket				_client;
			blc::tools::protocolFactory<int, std::string>	_admProto;
			std::string					_userKey;
			std::string					_id_msg;
		};
	}
}