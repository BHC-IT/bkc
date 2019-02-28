#include "chain.hpp"
#include "identity.hpp"

bkc::chain::chain(blc::tools::pipe &pipe, std::string name, bkc::rsaKey key, unsigned char admLvl, std::string in, std::string out) : actor(pipe, name), _admKey(key), _admLvl(admLvl), _in(in), _out(out)
{
	this->masterProto();

	this->_admKeyStr = this->_admKey.printablePub();
	this->load();
	this->start();
}

std::string bkc::chain::serialize() const
{
	return (this->_book.serialize());
}

void bkc::chain::unserialize(const std::string &str)
{
	this->_book.unserialize(str);
}

double bkc::chain::getBalance(const std::string &key) const
{
	std::vector<bkc::trans> v = this->_book.getByReceiver(key);
	double balance = 0;

	for (auto it : v){
		if (this->_book.consumed(it.getSign()) == false)
			balance += it.getAmount();
	}
	return (balance);
}

std::string bkc::chain::searchProof(const bkc::trans &t) const
{
	std::vector<bkc::trans> v = this->_book.getByReceiver(t.getSender());

	for (auto it : v){
		if (it.getAmount() >= t.getAmount() && this->_book.consumed(it.getSign()) == false)
			return (it.getSign());
	}
	return ("");
}

bkc::trans bkc::chain::getLeftOver(const bkc::trans &t) const
{
	bkc::trans parity;
	if (t.getProof() == "")
		return (parity);
	bkc::trans 		proof(this->_book.getBySign(t.getProof()));
	double			already_spent = 0;
	double			tmp;

	tmp = proof.getAmount() - t.getAmount();
	parity = bkc::trans::createTrans(proof.getReceiver(), proof.getReceiver(), round(tmp * 1000.0) / 1000.0, bkc::myLog);
	parity.setProof(t.getProof());
	return (parity);
}

bkc::trans bkc::chain::consum(const std::string &sign)
{
	bkc::trans 		proof(this->_book.getBySign(sign));
	std::vector<bkc::trans>	tmp(this->_book.getByProof(sign));
	double			already_spent = 0;

	for (auto it : tmp){
		already_spent += it.getAmount();
	}
	bkc::trans t = bkc::trans::createTrans(proof.getSender(), proof.getSender(), proof.getAmount() - already_spent, bkc::myLog);
	t.setProof(sign);
	this->add(t);
	return (t);
}

bool bkc::chain::verify(const bkc::trans &t)
{
	bkc::trans proof = this->_book.getBySign(t.getProof());

	if (t.getSender() != proof.getReceiver())
		return (false);
	if (t.check() == false)
		return (false);
	if (this->_book.exist(t.getProof()) == false)
		return (false);
	if (this->_book.consumed(t.getProof()) == true && t.getAmount() != 0)
		return (false);
	return (true);
}

double bkc::chain::leftOver(const bkc::trans &t, const bkc::trans &parity)
{
	bkc::trans proof = this->_book.getBySign(t.getProof());

	return (proof.getAmount() - (t.getAmount() + parity.getAmount()));
}

void bkc::chain::add(const bkc::trans &t)
{
	this->_book.add(t);
}

void bkc::chain::remove(const bkc::trans &t)
{
	this->_book.remove(t);
}

void bkc::chain::dump() const
{
	std::ofstream os;

	os.open(this->_out);
	this->_book.dump(os);
	os.close();
}

void bkc::chain::load()
{
	std::ifstream is;

	is.open(this->_in);
	this->_book.load(is);
	is.close();
}

void bkc::chain::readMaster()
{
	int code = std::stoi(this->_pipe.read());
	std::string tmp = this->_pipe.read();

	if (this->_masterProto.activate(code, tmp) == 280)
		this->kill();
}

void bkc::chain::thick()
{
	static int i = 0;

	if (this->_pipe.readable()) {
		this->readMaster();
	}
	if (i == 1000){
		this->dump();
		i = 0;
	}
}
