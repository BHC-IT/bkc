#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

namespace bfc {
	class flags {
	public:
		static void		init(const std::vector<std::string> &av);
		static bool		isSet(std::string key);
		static std::string	getValue(std::string key);
		static void		call(std::string key, std::function<void(std::string)> callback, std::function<void()> error = [](){});
		static bool		check(std::vector<std::string> req);
	private:
		flags() = default;
		flags(const flags &other) = delete;
		flags(flags &&other) = delete;
		flags		&operator=(const flags &other) = delete;
		flags		&operator=(flags &&other) = delete;
		static flags	&hold();
		static void	emplace(std::string key, std::string value);

		static std::map<std::string, std::string> _av;
	};
}