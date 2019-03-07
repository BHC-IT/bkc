#include <iostream>
#include <fstream>
#include <blc/serializable.hpp>
#include "lists.hpp"

std::vector<std::string>	bkc::lists::_black;
std::vector<std::string>	bkc::lists::_white;
bool				bkc::lists::_blackList;
bool				bkc::lists::_whiteList;

bool bkc::lists::isOk(const std::string &key)
{
	if (bkc::lists::_whiteList){
		if (bkc::lists::isOnWhiteList(key))
			return (true);
		else
			return (false);
	}
	if (bkc::lists::_blackList){
		if (bkc::lists::isOnBlackList(key))
			return (false);
	}
	return (true);
}

void bkc::lists::addOnList(const std::string &key)
{
	if (bkc::lists::_whiteList)
		bkc::lists::addOnWhiteList(key);
	else if (bkc::lists::_blackList)
		bkc::lists::addOnBlackList(key);
}

bool bkc::lists::isOnBlackList(const std::string &key)
{
	for (auto it : bkc::lists::_black){
		if (it == key)
			return (true);
	}
	return (false);
}

void bkc::lists::addOnBlackList(const std::string &key)
{
	bkc::lists::_black.push_back(key);
}

bool bkc::lists::isOnWhiteList(const std::string &key)
{
	for (auto it : bkc::lists::_white){
		if (it == key)
			return (true);
	}
	return (false);
}

void bkc::lists::addOnWhiteList(const std::string &key)
{
	bkc::lists::_white.push_back(key);
}

bool bkc::lists::isBlackList()
{
	return (bkc::lists::_blackList);
}

void bkc::lists::setBlackList()
{
	bkc::lists::_blackList = true;
}


bool bkc::lists::isWhiteList()
{
	return (bkc::lists::_whiteList);
}


void bkc::lists::setWhiteList()
{
	bkc::lists::_whiteList = true;
}

void bkc::lists::load(const std::string &filename)
{
	std::ifstream is;
	std::string tmp;

	try {
		is.open(filename);
		for (; std::getline(is, tmp);)
			bkc::lists::addOnList(tmp);
		is.close();
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
	}
}

std::string bkc::lists::serialize()
{
	std::string tmp;
	if (bkc::lists::_whiteList){
		for (auto it : bkc::lists::_white){
			tmp += it + ";";
		}
	} else if (bkc::lists::_blackList) {
		for (auto it : bkc::lists::_black){
			tmp += it + ";";
		}
	}
	return (tmp);
}

void bkc::lists::unserialize(const std::string &str, bool white, bool black)
{
	std::string data = str;
	if (white){
		for (std::string tmp = blc::tools::serializable::cut(data, ';'); data != ""; tmp = blc::tools::serializable::cut(data, ';'))
			bkc::lists::addOnWhiteList(tmp);
	} else if (black) {
		for (std::string tmp = blc::tools::serializable::cut(data, ';'); data != ""; tmp = blc::tools::serializable::cut(data, ';'))
			bkc::lists::addOnBlackList(tmp);
	}
}
