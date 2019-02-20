#pragma once

#include <blc/blc.hpp>

namespace bkc {
	class connectedPeer : blc::tools::serializable {
	public:
		static connectedPeer		&init();
		static std::vector<std::string> getConnected();
		static bool			isConnected(const std::string &str);
		static bool			isConnected(const std::string &str, int port);
		static bool			connect(const std::string &str);
		static bool			connect(const std::string &str, int port);
		static void			erase(const std::string &str);
		static void			erase(const std::string &str, int port);
		unsigned int			size();
		std::string			serialize() const;
		void				unserialize(const std::string &str);
	private:
		connectedPeer() = default;
		static std::vector<std::string> _con;
		static std::mutex		_access;
	};
}