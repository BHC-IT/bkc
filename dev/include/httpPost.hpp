#pragma once

#include <blc/blc.hpp>
#include <string>
#include <vector>

namespace bkc {
	namespace node {
		class httpPost : public blc::tools::serializable {
		public:
			httpPost();
			httpPost(const std::vector<std::string> &v);
			httpPost(const httpPost &other);
			httpPost(httpPost &&other);
			~httpPost() = default;

			std::string				getHost() const;
			std::string				getVersion() const;
			std::string				getOrigin() const;
			std::string				getAgent() const;
			std::string				getContentType() const;
			int					getLength() const;
			std::map<std::string, std::string>	getValue() const;

			std::map<std::string, std::string>::iterator	begin();
			std::map<std::string, std::string>::iterator	end();
			std::map<std::string, std::string>::iterator	find(std::string str);
			int						size() const;

			httpPost &			parse(const std::vector<std::string> v);
			std::vector<std::string> 	dump();
			std::string 			serialize() const;
			void 				unserialize(const std::string &str);

			httpPost	&operator=(const std::vector<std::string> v);
			httpPost	&operator=(const httpPost &other);
			httpPost	&operator=(httpPost &&other);
			void		clear();
		private:
			std::string				_host;
			std::string				_version;
			std::string				_origin;
			std::string				_agent;
			std::string				_contentType;
			int					_length;
			std::map<std::string, std::string>	_value;
		};
		bool isPost(std::vector<std::string> v);
	}
}