#include <blc/serializable.hpp>
#include <blc/errorHandle.hpp>
#include "bfc/flag.hpp"

std::map<std::string, std::string>	bfc::flags::_av;

void bfc::flags::init(const std::vector<std::string> &av)
{
	if (_av.size())
		throw blc::error::exception("flags is already set and can't be re-seted");
	for (auto i = av.begin() + 1; i != av.end(); i++) {
		std::string str = *i;

		if (str.size() >= 2) {
			if (str[0] == '-' && str[1] == '-'){
				str.erase(str.begin(), str.begin() + 2);
				std::string tmp = blc::tools::serializable::cut(str, '=');
				bfc::flags::emplace(tmp, str);
			} else if (str[0] == '-') {
				str.erase(str.begin());
				if ((str >= "A" && str <= "Z") || str.size() == 1){
					std::string tmp = *(i + 1);

					bfc::flags::emplace(str, tmp);
					i++;
				} else {
					for (char c : str) {
						// std::string tmp = "" + c; // behave weirdly, must invastigate.
						std::string tmp;

						tmp += c;
						bfc::flags::emplace(tmp, tmp);
					}
				}
			}
		}
	}
}

bool bfc::flags::isSet(std::string key)
{
	if (_av.find(key) != _av.end())
		return (true);
	return (false);
}

std::string bfc::flags::getValue(std::string key)
{
	if (bfc::flags::isSet(key) == false)
		return ("");
	return (_av[key]);
}

bfc::flags &bfc::flags::hold()
{
	static bfc::flags flags;

	return (flags);
}

void bfc::flags::emplace(std::string key, std::string value)
{
	_av.emplace(key, value);
}

void bfc::flags::call(std::string key, std::function<void(std::string)> callback, std::function<void()> error)
{
	if (bfc::flags::isSet(key) == true)
		callback(bfc::flags::getValue(key));
	else
		error();
}

bool bfc::flags::check(std::vector<std::string> req)
{
	for (auto it : req){
		if (bfc::flags::isSet(it) == false)
			return (false);
	}
	return (true);
}
