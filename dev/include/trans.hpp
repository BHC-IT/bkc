#pragma once

#include <blc/blc.hpp>
#include <ctime>
#include <nlohmann/json.hpp>
#include "rsaKey.hpp"

using json = nlohmann::json;

namespace bkc {
	class trans : public blc::tools::serializable {
	public:
		trans() : _timestamp(std::time(nullptr)), _amount(0){};
		trans(std::string str);
		trans(const json &j);
		trans(const trans &other) = default;

		bool		check() const;
		double		leftOver() const;
		bool 		toSelf() const;

		std::string	serialize() const;
		void		unserialize(const std::string &str);
		json		jsonify() const;
		std::string	debug() const;

		int		getTimestamp() const;
		double		getAmount() const;
		std::string	getSender() const;
		std::string	getReceiver() const;
		std::string	getSign() const;
		std::string	getProof() const;

		void		setProof(const std::string &str);

		trans		&operator=(const trans &other);
		bool		operator==(const trans &other);

		static trans	createTrans(const std::string &from, const std::string &to, double amount, const bkc::rsaKey &key);
	private:
		int			_timestamp;
		double			_amount;
		std::string		_sender;
		std::string		_receiver;
		std::string		_sign;
		std::string		_proof;
	};
}