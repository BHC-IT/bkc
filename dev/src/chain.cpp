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
	double tot_amout = 0;
	std::string signature;
	for (auto it : v){
		tot_amout += it.getAmount();
		if (this->_book.consumed(it.getSign()) == false){
			if (signature == "")
				signature = it.getSign();
			else
				signature += "/" + it.getSign();
		}
		if (tot_amout >= t.getAmount()){
			return (signature);
		}
	}
	return ("");
}

bkc::trans bkc::chain::getLeftOver(const bkc::trans &t) const
{
	bkc::trans parity;
	if (t.getProof() == "")
		return (parity);
	std::vector<bkc::trans> proofs = this->_book.getAllProof(t.getProof());
	double			already_spent = 0;
	double			to_spend = 0;
	double			tmp;

	for (auto it : proofs){
		to_spend += it.getAmount();
	}
	tmp = to_spend - t.getAmount();
	parity = bkc::trans::createTrans(t.getSender(), t.getSender(), round(tmp * 1000.0) / 1000.0, bkc::myLog);
	parity.setProof(t.getProof());
	return (parity);
}

bkc::trans bkc::chain::consum(const std::string &sign)
{
	std::vector<bkc::trans> proofs = this->_book.getAllProof(sign);
	std::vector<bkc::trans>	tmp(this->_book.getByProof(sign));
	double			already_spent = 0;
	double			to_spend = 0;

	for (auto it : tmp){
		already_spent += round(it.getAmount()) * 1000;
	}
	for (auto it : proofs){
		to_spend += round(it.getAmount()) * 1000;
	}
	bkc::trans t = bkc::trans::createTrans(proofs[0].getSender(), proofs[0].getSender(), to_spend - already_spent, bkc::myLog);
	t.setProof(sign);
	this->add(t);
	return (t);
}

bool bkc::chain::verify(const bkc::trans &t)
{
	std::vector<bkc::trans> proofs = this->_book.getAllProof(t.getProof());

	if (t.check() == false)
		return (false);
	for (auto it : proofs){
		if (t.getSender() != it.getReceiver()){
			return (false);
		}
		if (this->_book.exist(it.getSign()) == false){
			return (false);
		}
		if (this->_book.consumed(it.getSign()) == true && t.getAmount() != 0){
			return (false);
		}
	}
	return (true);
}

double bkc::chain::leftOver(const bkc::trans &t, const bkc::trans &parity)
{
	std::vector<bkc::trans> proofs = this->_book.getAllProof(t.getProof());
	double amount = 0;
	int tmpTrans = round(t.getAmount() * 1000.0);
	int tmpParit = round(parity.getAmount() * 1000.0);

	for (auto it : proofs){
		std::cout << "amount : " << it.getAmount() << std::endl;
		amount += it.getAmount() * 1000;
	}
	std::cout << std::endl << tmpTrans << std::endl;
	std::cout << tmpParit << std::endl;
	std::cout << amount << std::endl;
	return (amount - (tmpTrans + tmpParit));
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
