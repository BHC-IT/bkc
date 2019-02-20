#include "bfc/usage.hpp"

bfc::base_usage bfc::usage;

void bfc::base_usage::add(std::vector<std::string> comm, std::string detail)
{
	this->_comm.emplace(std::make_pair(comm, detail));
}

std::string bfc::base_usage::serialize() const
{
	std::string ret;

	return (ret);
}

void bfc::base_usage::unserialize(const std::string &str)
{

}
