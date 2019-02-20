#pragma once

#include <vector>
#include <map>
#include <string>
#include <blc/blc.hpp>
#include "bfc/outColor.hpp"

namespace bfc {
	class base_usage : blc::tools::serializable {
	public:
		base_usage() = default;
		base_usage(std::map<std::vector<std::string>, std::string> comm, std::string name) : _comm(comm), _procName(name){};
		base_usage(const base_usage &other) = default;
		void			add(std::vector<std::string> comm, std::string detail);
		void			setProcessName(std::string name){this->_procName = name;};
		std::string		serialize() const;
		void			unserialize(const std::string &str);
		friend std::ostream	&operator<<(std::ostream &os, const base_usage &usage){
				int tab = 0;
				for (auto comm : usage._comm){
					int size = 0;
					for (auto it : comm.first){
						size += it.size();
					}
					if (size / 8 >= tab / 8){
						tab = size / 8;
					}
				}
				os << bfc::color::modifier(bfc::color::FG_GREEN) << "[USAGE]:\n\t\t" << bfc::color::modifier(bfc::color::FG_DEFAULT);
				os << usage._procName << std::endl << std::endl;
				for (auto comm : usage._comm){
					int size = 0;
					for (auto it : comm.first){
						if (size != 0){
							size += 2;
							os << ", ";
						}
						size += it.size();
						os << it;
					}
					for (int i = 0; i != 4 - (size / 8); i++){
						os << "\t";
					}
					os << "\t\t";
					os << comm.second << std::endl;
				}
				return (os);
		};
	private:
		std::map<std::vector<std::string>, std::string>	_comm;
		std::string					_procName;
	};
	extern base_usage usage;
}
