#include "trans.hpp"

bkc::trans::trans(std::string str)
{
	if (str != ""){
		json j = json::parse(str);

		try {
			this->_sender = j["sender"];
			this->_receiver = j["receiver"];
			this->_amount = j["amount"];
			this->_timestamp = j["timestamp"];
			this->_sign = j["sign"];
		} catch (blc::error::exception &e){
			throw blc::error::exception(assertError(e.what()));
		}
		if (j.count("proof"))
			this->_proof = j["proof"];
	}
}

bkc::trans::trans(const json &j)
{
	try {
		this->_sender = j["sender"];
		this->_receiver = j["receiver"];
		this->_amount = j["amount"];
		this->_timestamp = j["timestamp"];
		this->_sign = j["sign"];
	} catch (blc::error::exception &e){
		throw blc::error::exception(assertError(e.what()));
	}
	if (j.count("proof"))
		this->_proof = j["proof"];
}

bool bkc::trans::check() const
{
	rsaKey key;
	json j({
		{"sender", this->_sender},
		{"receiver", this->_receiver},
		{"amount", this->_amount},
		{"timestamp", this->_timestamp}
	});

	if (this->_amount < 0)
		return (false);
	key.importPub(this->_sender);
	return (key.verifyPrintable(j.dump(), this->_sign));
}

double bkc::trans::leftOver() const
{
	bkc::trans t(this->_proof);

	return (t.getAmount() - this->_amount);
}

bool bkc::trans::toSelf() const
{
	return (this->_sender == this->_receiver);
}

std::string bkc::trans::serialize() const
{
	json j;

	j["sender"] = this->_sender;
	j["receiver"] = this->_receiver;
	j["amount"] = this->_amount;
	j["timestamp"] = this->_timestamp;
	j["sign"] = this->_sign;
	j["proof"] = this->_proof;

	return (j.dump());
}

void bkc::trans::unserialize(const std::string &str)
{
	try {
		json j = json::parse(str);
		this->_sender = j["sender"];
		this->_receiver = j["receiver"];
		this->_amount = j["amount"];
		this->_timestamp = j["timestamp"];
		this->_sign = j["sign"];
		if (j.count("proof"))
			this->_proof = j["proof"];
	} catch (blc::error::exception &e){
		throw blc::error::exception(assertError(e.what()));
	} catch (nlohmann::detail::parse_error &e){
		throw blc::error::exception(assertError(e.what()));
	}
}

json bkc::trans::jsonify() const
{
	json j;

	j["sender"] = this->_sender;
	j["receiver"] = this->_receiver;
	j["amount"] = this->_amount;
	j["timestamp"] = this->_timestamp;
	j["sign"] = this->_sign;

	return (j);
}

std::string bkc::trans::debug() const
{
	json j;

	j["sender"] = this->_sender;
	j["receiver"] = this->_receiver;
	j["amount"] = this->_amount;
	j["timestamp"] = this->_timestamp;

	return (j.dump());
}

int bkc::trans::getTimestamp() const
{
	return (this->_timestamp);
}

double bkc::trans::getAmount() const
{
	return (this->_amount);
}

std::string bkc::trans::getSender() const
{
	return (this->_sender);
}

std::string bkc::trans::getReceiver() const
{
	return (this->_receiver);
}

std::string bkc::trans::getSign() const
{
	return (this->_sign);
}

std::string bkc::trans::getProof() const
{
	return (this->_proof);
}


void bkc::trans::setProof(const std::string &str)
{
	this->_proof = str;
}

bkc::trans &bkc::trans::operator=(const trans &other)
{
	this->_timestamp = other.getTimestamp();
	this->_amount = other.getAmount();
	this->_sender = other.getSender();
	this->_receiver = other.getReceiver();
	this->_sign = other.getSign();
	this->_proof = other.getProof();
	return (*this);
}

bool bkc::trans::operator==(const trans &other)
{
	if (this->_sign != other.getSign())
		return false;
	if (this->_timestamp != other.getTimestamp())
		return false;
	if (this->_amount != other.getAmount())
		return false;
	if (this->_receiver != other.getReceiver())
		return false;
	if (this->_sender != other.getSender())
		return false;
	return true;
}

bkc::trans bkc::trans::createTrans(const std::string &from, const std::string &to, double amount, const bkc::rsaKey &key)
{
	json	j;

	j["sender"] = from;
	j["receiver"] = to;
	j["amount"] = amount;
	j["timestamp"] = std::time(nullptr);
	j["sign"] = key.signPrintable(j.dump());

	bkc::trans t(j);

	return (t);
}
