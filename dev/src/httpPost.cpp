#include <utility>
#include "httpPost.hpp"

bkc::node::httpPost::httpPost() :	_host(""), 						_version("1.1"),
					_origin(""),						_agent(""),
					_contentType("application/x-www-form-urlencoded"),	_length(0)
{
}

bkc::node::httpPost::httpPost(const std::vector<std::string> &v)
{
	if (bkc::node::isPost(v) == false)
		return;
	std::string tmp = v[0];
	std::string trash;

	trash = blc::tools::serializable::cut(tmp, ' ');
	this->_host = blc::tools::serializable::cut(tmp, ' ');
	blc::tools::serializable::cut(tmp, ' ');
	this->_version = tmp;

	int i;

	for (i = 1; i != v.size(); i++){
		tmp = v[i];
		trash = blc::tools::serializable::cut(tmp, ' ');
		if (tmp[tmp.size() - 1] == '\r')
			tmp = blc::tools::serializable::cut(tmp, '\r');
		if (trash == "From:")
			this->_origin = tmp;
		if (trash == "User-Agent:")
			this->_agent = tmp;
		if (trash == "Content-Type:")
			this->_contentType = tmp;
		if (trash == "Content-Length:"){
			this->_length = std::stoi(tmp);
		}
	}

	return;
}

bkc::node::httpPost::httpPost(const bkc::node::httpPost &other) :	_host(other.getHost()), 		_version(other.getVersion()),
									_origin(other.getOrigin()),		_agent(other.getAgent()),
									_contentType(other.getContentType()),	_length(other.getLength()),
									_value(other.getValue())
{
}

bkc::node::httpPost::httpPost(bkc::node::httpPost &&other) : 	_host(other.getHost()),			_version(other.getVersion()),
								_origin(other.getOrigin()), 		_agent(other.getAgent()),
								_contentType(other.getContentType()),	_length(other.getLength()),
								_value(other.getValue())
{
	other.clear();
}

std::string bkc::node::httpPost::getHost() const
{
	return (this->_host);
}

std::string bkc::node::httpPost::getVersion() const
{
	return (this->_version);
}

std::string bkc::node::httpPost::getOrigin() const
{
	return (this->_origin);
}

std::string bkc::node::httpPost::getAgent() const
{
	return (this->_agent);
}

std::string bkc::node::httpPost::getContentType() const
{
	return (this->_contentType);
}

int bkc::node::httpPost::getLength() const
{
	return (this->_length);
}

std::map<std::string, std::string> bkc::node::httpPost::getValue() const
{
	return (this->_value);
}

std::map<std::string, std::string>::iterator bkc::node::httpPost::begin()
{
	return (this->_value.begin());
}

std::map<std::string, std::string>::iterator bkc::node::httpPost::end()
{
	return (this->_value.end());
}

std::map<std::string, std::string>::iterator bkc::node::httpPost::find(std::string str)
{
	return (this->_value.find(str));
}

int bkc::node::httpPost::size() const
{
	return (this->_value.size());
}

bkc::node::httpPost &bkc::node::httpPost::parse(const std::vector<std::string> v)
{
	if (bkc::node::isPost(v) == false)
		return (*this);
	std::string tmp = v[0];
	std::string trash;

	trash = blc::tools::serializable::cut(tmp, ' ');
	this->_host = blc::tools::serializable::cut(tmp, ' ');
	blc::tools::serializable::cut(tmp, ' ');
	this->_version = tmp;

	int i;

	for (i = 1; i != v.size(); ++i){
		tmp = v[i];
		trash = blc::tools::serializable::cut(tmp, ' ');
		if (trash == "From:")
			this->_origin = tmp;
		if (trash == "User-Agent:")
			this->_agent = tmp;
		if (trash == "Content-Type:")
			this->_contentType = tmp;
		if (trash == "Content-Length:"){
			this->_length = std::stoi(tmp);
			i++;
			break;
		}
	}
	while (i != v.size() && v[i] == "")
		i++;

	tmp = v[i];
	while (tmp.find('=') != std::string::npos){
		std::string key = blc::tools::serializable::cut(tmp, '=');
		std::string value = blc::tools::serializable::cut(tmp, '&');

		this->_value[key] = value;
	}

	return (*this);
}

std::vector<std::string> bkc::node::httpPost::dump()
{
	std::vector<std::string>	v;
	std::string 			tmp;

	tmp = "POST ";
	tmp += this->_host;
	tmp += " HTTP/";
	tmp += this->_version;
	v.push_back(tmp);
	tmp.clear();

	tmp = "From: ";
	tmp += this->_origin;
	v.push_back(tmp);
	tmp.clear();

	tmp = "User-Agent: ";
	tmp += this->_agent;
	v.push_back(tmp);
	tmp.clear();

	tmp = "Content-Type: ";
	tmp += this->_contentType;
	v.push_back(tmp);
	tmp.clear();

	tmp = "Content-Length: ";
	tmp += std::to_string(this->_length);
	v.push_back(tmp);
	tmp.clear();

	v.push_back(tmp);

	for (auto it = this->_value.begin(); it!= this->_value.end(); ++it){
		tmp += it->first;
		tmp += "=";
		tmp += it->second;
		tmp += "&";
	}
	tmp.pop_back();
	v.push_back(tmp);

	return (v);
}

std::string bkc::node::httpPost::serialize() const
{
	std::string ret;

	ret = this->_host;
	ret += ";";
	ret += this->_version;
	ret += ";";
	ret += this->_origin;
	ret += ";";
	ret += this->_agent;
	ret += ";";
	ret += this->_contentType;
	ret += ";";
	ret += std::to_string(this->_length);
	ret += ";";
	for (auto it = this->_value.begin(); it!= this->_value.end(); ++it){
		ret += it->first;
		ret += "=";
		ret += it->second;
		ret += "&";
	}
	ret.pop_back();

	return (ret);
}

void bkc::node::httpPost::unserialize(const std::string &str)
{
	std::string tmp = str;

	this->_host = blc::tools::serializable::cut(tmp, ';');
	this->_version = blc::tools::serializable::cut(tmp, ';');
	this->_origin = blc::tools::serializable::cut(tmp, ';');
	this->_agent = blc::tools::serializable::cut(tmp, ';');
	this->_contentType = blc::tools::serializable::cut(tmp, ';');
	this->_length = std::stoi(blc::tools::serializable::cut(tmp, ';'));
	while (tmp.find(';') != std::string::npos){
		this->_value.emplace(std::make_pair(blc::tools::serializable::cut(tmp, '='), blc::tools::serializable::cut(tmp, '&')));
	}
}

bkc::node::httpPost &bkc::node::httpPost::operator=(const std::vector<std::string> v)
{
	if (bkc::node::isPost(v) == false)
		return (*this);
	std::string tmp = v[0];
	std::string trash;

	trash = blc::tools::serializable::cut(tmp, ' ');
	this->_host = blc::tools::serializable::cut(tmp, ' ');
	blc::tools::serializable::cut(tmp, ' ');
	this->_version = tmp;

	int i;

	for (i = 1; i != v.size(); ++i){
		tmp = v[i];
		trash = blc::tools::serializable::cut(tmp, ' ');
		if (trash == "From:")
			this->_origin = tmp;
		if (trash == "User-Agent:")
			this->_agent = tmp;
		if (trash == "Content-Type:")
			this->_contentType = tmp;
		if (trash == "Content-Length:"){
			this->_length = std::stoi(tmp);
			i++;
			break;
		}
	}
	while (i != v.size() && v[i] == "")
		i++;

	tmp = v[i];
	while (tmp.find('=') != std::string::npos){
		std::string key = blc::tools::serializable::cut(tmp, '=');
		std::string value = blc::tools::serializable::cut(tmp, '&');

		this->_value[key] = value;
	}

	return (*this);
}

bkc::node::httpPost &bkc::node::httpPost::operator=(const bkc::node::httpPost &other)
{
	this->_host = other.getHost();
	this->_version = other.getVersion();
	this->_origin = other.getOrigin();
	this->_agent = other.getAgent();
	this->_contentType = other.getContentType();
	this->_length = other.getLength();
	this->_value = other.getValue();

	return (*this);
}

bkc::node::httpPost &bkc::node::httpPost::operator=(bkc::node::httpPost &&other)
{

	this->_host = other.getHost();
	this->_version = other.getVersion();
	this->_origin = other.getOrigin();
	this->_agent = other.getAgent();
	this->_contentType = other.getContentType();
	this->_length = other.getLength();
	this->_value = other.getValue();

	other.clear();

	return (*this);
}

void bkc::node::httpPost::clear()
{
	this->_host = "";
	this->_version = "";
	this->_origin = "";
	this->_agent = "";
	this->_contentType = "";
	this->_length = 0;
	this->_value.clear();
}

bool bkc::node::isPost(std::vector<std::string> v)
{
	if (v.size() < 1)
		return (false);
	if (v[0].size() < 4)
		return (false);
	if (v[0][0] == 'P' && v[0][1] == 'O' && v[0][2] == 'S' && v[0][3] == 'T')
		return (true);
	return (false);
}