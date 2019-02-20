#pragma once

#include <blc/blc.hpp>
#include <bfc/bfc.hpp>

#include "book.hpp"
#include "trans.hpp"
#include "rsaKey.hpp"

namespace bkc {
	class chain : public bfc::actor<chain>, public blc::tools::serializable {
	public:
		chain() = delete;
		chain(blc::tools::pipe &pipe, std::string name, bkc::rsaKey key, unsigned char admLvl, std::string in, std::string out);

		void masterProto();

		void readMaster();
		void thick();

		std::string 	serialize() const;
		void		unserialize(const std::string &str);
		double		getBalance(const std::string &key) const;
		std::string	searchProof(const bkc::trans &t) const;
		bkc::trans	getLeftOver(const bkc::trans &t) const;
		bkc::trans	consum(const std::string &sign);
		void		dump() const;
	private:
		bool		verify(const bkc::trans &t);
		double		leftOver(const bkc::trans &t, const bkc::trans &parity);
		void		add(const bkc::trans &t);
		void		remove(const bkc::trans &t);
		void		load();
		unsigned char				_admLvl;
		bkc::rsaKey				_admKey;
		std::string				_admKeyStr;
		bkc::book				_book;
		std::mutex				_access;
		std::string				_in;
		std::string				_out;
	};
}