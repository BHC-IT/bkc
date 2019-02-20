#include "book.hpp"

bkc::book::book()
{
}

bkc::book::book(std::string filename)
{
	std::ifstream stream;

	stream.open(filename);
	this->load(stream);
	stream.close();
}

bkc::book::book(const bkc::book &other)
{
	for (auto it : other._bySign){
		this->add(it.second);
	}
}

void bkc::book::add(const bkc::trans &t)
{
	this->_bySign[t.getSign()] = t;
	this->_byReceiver[t.getReceiver()].push_back(&this->_bySign[t.getSign()]);
	this->_bySender[t.getSender()].push_back(&this->_bySign[t.getSign()]);
	this->_byProof[t.getProof()].push_back(&this->_bySign[t.getSign()]);
	this->_byAmount[t.getAmount()].push_back(&this->_bySign[t.getSign()]);
	this->_byTime[t.getTimestamp()].push_back(&this->_bySign[t.getSign()]);
}

void bkc::book::remove(const bkc::trans &t)
{
	for (auto it = this->_bySender[t.getSender()].begin(); it != this->_bySender[t.getSender()].end(); it++){
		if (**it == t)
			this->_bySender[t.getSender()].erase(it);
	}
	for (auto it = this->_byReceiver[t.getReceiver()].begin(); it != this->_byReceiver[t.getReceiver()].end(); it++){
		if (**it == t)
			this->_byReceiver[t.getReceiver()].erase(it);
	}
	for (auto it = this->_byProof[t.getProof()].begin(); it != this->_byProof[t.getProof()].end(); it++){
		if (**it == t)
			this->_byProof[t.getProof()].erase(it);
	}
	for (auto it = this->_byAmount[t.getAmount()].begin(); it != this->_byAmount[t.getAmount()].end(); it++){
		if (**it == t)
			this->_byAmount[t.getAmount()].erase(it);
	}
	for (auto it = this->_byTime[t.getTimestamp()].begin(); it != this->_byTime[t.getTimestamp()].end(); it++){
		if (**it == t)
			this->_byTime[t.getTimestamp()].erase(it);
	}
	this->_bySign.erase(t.getSign());
}

bool bkc::book::find(const bkc::trans &t)
{
	return (this->_bySign.count(t.getSign()) != 0);
}

void bkc::book::clear()
{
	this->_bySign.clear();
	this->_byReceiver.clear();
	this->_bySender.clear();
	this->_byProof.clear();
	this->_byAmount.clear();
	this->_byTime.clear();
}

bool bkc::book::exist(const std::string &sign)
{
	if (this->_bySign.count(sign) == 0)
		return (false);
	return (true);
}

bool bkc::book::consumed(const std::string &sign) const
{
	bkc::trans 		proof(this->getBySign(sign));
	std::vector<bkc::trans>	tmp(this->getByProof(sign));
	double			already_spent = 0;

	for (auto it : tmp){
		already_spent += it.getAmount();
	}
	if (already_spent >= proof.getAmount())
		return (true);
	return (false);
}


std::vector<bkc::trans> bkc::book::getBySender(const std::string &key) const
{
	std::vector<bkc::trans> v;
	if (this->_bySender.count(key) == 0)
		return (std::vector<bkc::trans>({}));
	for (auto it : this->_bySender.at(key)){
		v.push_back(*it);
	}
	return (v);
}

std::vector<bkc::trans> bkc::book::getByReceiver(const std::string &key) const
{
	std::vector<bkc::trans> v;
	if (this->_byReceiver.count(key) == 0)
		return (std::vector<bkc::trans>({}));
	for (auto it : this->_byReceiver.at(key)){
		v.push_back(*it);
	}
	return (v);
}

bkc::trans bkc::book::getBySign(const std::string &key) const
{
	bkc::trans t;
	if (this->_bySign.count(key) == 0)
		return (t);
	return (this->_bySign.at(key));
}

std::vector<bkc::trans> bkc::book::getByProof(const std::string &key) const
{
	std::vector<bkc::trans> v;
	if (this->_byProof.count(key) == 0)
		return (std::vector<bkc::trans>({}));
	for (auto it : this->_byProof.at(key)){
		v.push_back(*it);
	}
	return (v);
}

std::vector<bkc::trans> bkc::book::getByAmount(double amount) const
{
	std::vector<bkc::trans> v;
	if (this->_byAmount.count(amount) == 0)
		return (std::vector<bkc::trans>({}));
	for (auto it : this->_byAmount.at(amount)){
		v.push_back(*it);
	}
	return (v);
}

std::vector<bkc::trans> bkc::book::getByTime(int time) const
{
	std::vector<bkc::trans> v;
	if (this->_byTime.count(time) == 0)
		return (std::vector<bkc::trans>({}));
	for (auto it : this->_byTime.at(time)){
		v.push_back(*it);
	}
	return (v);
}

std::string bkc::book::serialize() const
{
	std::string str;

	for (auto it : this->_bySign){
		str += it.second.serialize();
		str += ';';
	}
	return (str);
}

void bkc::book::unserialize(const std::string &str)
{
	std::string data = str;

	this->clear();
	for (std::string tmp = blc::tools::serializable::cut(data, ';'); tmp != ""; tmp = blc::tools::serializable::cut(data, ';')){
		bkc::trans t(tmp);

		this->add(t);
	}
}

void bkc::book::load(std::istream &stream)
{
	std::string str;

	this->clear();
	for (std::getline(stream, str); str != ""; std::getline(stream, str)){
		bkc::trans t;

		t.unserialize(str);
		this->add(t);
	}
}

void bkc::book::dump(std::ostream &stream) const
{
	for (auto it : this->_bySign){
		stream << it.second.serialize() << std::endl;
	}
}

bkc::book &bkc::book::operator=(const bkc::book &other)
{
	this->clear();
	for (auto it : other._bySign){
		this->add(it.second);
	}
	return (*this);
}