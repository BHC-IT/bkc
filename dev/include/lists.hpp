#pragma once

#include <vector>
#include <string>

namespace bkc {
	class lists {
	public:
		lists() = delete;
		static bool isOk(const std::string &key);
		static void addOnList(const std::string &key);
		static bool isOnBlackList(const std::string &key);
		static void addOnBlackList(const std::string &key);
		static bool isOnWhiteList(const std::string &key);
		static void addOnWhiteList(const std::string &key);
		static bool isBlackList();
		static void setBlackList();
		static bool isWhiteList();
		static void setWhiteList();
		static void load(const std::string &filename);
		static std::string	serialize();
		static void		unserialize(const std::string &str, bool white, bool black);
	private:
		static std::vector<std::string> _black;
		static std::vector<std::string> _white;
		static bool			_blackList;
		static bool			_whiteList;
	};
}