#pragma once

#include <blc/blc.hpp>
#include "trans.hpp"

namespace bkc {
	class book : public blc::tools::serializable {
	public:
		book();
		book(std::string filename);
		book(const book &other);

		void	add(const bkc::trans &t);
		void	remove(const bkc::trans &t);
		bool	find(const bkc::trans &t);
		void	clear();
		bool	exist(const std::string &sign);
		bool	consumed(const std::string &sign) const;

		std::vector<bkc::trans>	getBySender(const std::string &key) const;
		std::vector<bkc::trans>	getByReceiver(const std::string &key) const;
		bkc::trans		getBySign(const std::string &key) const;
		std::vector<bkc::trans>	getByProof(const std::string &key) const;
		std::vector<bkc::trans>	getByAmount(double amount) const;
		std::vector<bkc::trans>	getByTime(int time) const;

		std::vector<bkc::trans>	getAllProof(const std::string &keys);

		std::string 	serialize() const;
		void		unserialize(const std::string &str);
		void		load(std::istream &stream);
		void		dump(std::ostream &stream) const;

		book		&operator=(const book &other);
	private:
		std::map<std::string, std::vector<bkc::trans *>>	_bySender;
		std::map<std::string, std::vector<bkc::trans *>>	_byReceiver;
		std::map<std::string, bkc::trans>			_bySign;
		std::map<std::string, std::vector<bkc::trans *>>	_byProof;
		std::map<double, std::vector<bkc::trans *>>		_byAmount;
		std::map<int, std::vector<bkc::trans *>>		_byTime;
	};
}