#include "rsaKey.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

// bkc::rsaKey::rsaKey()
// {
// }

bkc::rsaKey::rsaKey(const std::string &pub, const std::string &priv) : _pub(pub), _priv(priv)
{
	int err = rsa_import((unsigned char *)pub.data(), pub.size(), &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception(assertError("cant import pub key"));
	err = rsa_import((unsigned char *)priv.data(), priv.size(), &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception(assertError("cant import private key"));
}

bkc::rsaKey::rsaKey(rsa_key key)
{
	std::memcpy(&this->_key, &key, sizeof(rsa_key));

	unsigned char key_str[bitsize * 5 / 8];
	unsigned long outlen = sizeof(key_str);
	int err = rsa_export(key_str, &outlen, PK_PRIVATE, &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception("Bad key format");
	this->_priv = std::string(key_str, key_str + outlen);

	outlen = sizeof(key_str);
	err = rsa_export(key_str, &outlen, PK_PUBLIC, &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception("Bad key format");
	this->_pub = std::string(key_str, key_str + outlen);
}

bkc::rsaKey::rsaKey(const bkc::rsaKey &other) : _key(other.getKey()), _pub(other.getPub()), _priv(other.getPriv())
{
}

bkc::rsaKey::~rsaKey()
{
	// rsa_free(&this->_key);
}

bkc::rsaKey &bkc::rsaKey::operator=(const bkc::rsaKey &other)
{
	rsa_key tmp = other.getKey();
	std::memcpy(&this->_key, &tmp, sizeof(rsa_key));
	this->_pub = other.getPub();
	this->_priv = other.getPriv();

	return (*this);
}

void bkc::rsaKey::setKey(rsa_key key)
{
	std::memcpy(&this->_key, &key, sizeof(rsa_key));

	unsigned char key_str[bitsize * 5 / 8];
	unsigned long outlen = sizeof(key_str);
	int err = rsa_export(key_str, &outlen, PK_PRIVATE, &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception("Bad key format");
	this->_priv = std::string(key_str, key_str + outlen);

	outlen = sizeof(key_str);
	err = rsa_export(key_str, &outlen, PK_PUBLIC, &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception("Bad key format");
	this->_pub = std::string(key_str, key_str + outlen);
}

void bkc::rsaKey::setPub(const std::string &pub)
{
	this->_pub = pub;
	int err = rsa_import((unsigned char *)this->_pub.data(), this->_pub.size(), &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception(assertError("cant import pub key"));
}

void bkc::rsaKey::setPriv(const std::string &priv)
{
	this->_priv = priv;
	int err = rsa_import((unsigned char *)this->_priv.data(), this->_priv.size(), &this->_key);
	if (err != CRYPT_OK) throw blc::error::exception(assertError("cant import private key"));
}

std::string bkc::rsaKey::sign(const std::string &msg) const
{
	const ltc_hash_descriptor& hash_desc = sha512_desc;
	const int hash_idx = register_hash(&hash_desc);
	if (hash_idx < 0) throw blc::error::exception("Invalid hash");

	unsigned char hash[64];
	hash_state md;
	hash_desc.init(&md);
	hash_desc.process(&md, (const unsigned char*)msg.c_str(), (unsigned long)msg.size());
	hash_desc.done(&md, hash);

	const int padding = LTC_PKCS_1_V1_5;
	const unsigned long saltlen = 0;

	const int prng_idx = padding == LTC_PKCS_1_PSS ? register_prng(&sprng_desc) : 0;
	if (prng_idx < 0) throw blc::error::exception("Invalid PRNG");

	unsigned char sig[MAX_RSA_SIZE / 8];
	unsigned long siglen = sizeof(sig);
	int err = rsa_sign_hash_ex(hash, hash_desc.hashsize, sig, &siglen, padding, NULL, prng_idx, hash_idx, saltlen, const_cast<rsa_key *>(&this->_key));
	if (err != CRYPT_OK) throw blc::error::exception("Error signing with hash");

	return (std::string(sig, sig + siglen));
}

std::string bkc::rsaKey::signPrintable(const std::string &msg) const
{
	std::string tmp = this->sign(msg);
	std::string ret;

	for (auto it : tmp)
		ret += std::to_string((int)it) + " ";
	return (ret);
}

bool bkc::rsaKey::verify(const std::string &msg, const std::string &sign) const
{
	const ltc_hash_descriptor& hash_desc = sha512_desc;
	const int hash_idx = register_hash(&hash_desc);
	if (hash_idx < 0) throw blc::error::exception("Invalid hash");

	unsigned char hash[64];
	hash_state md;
	hash_desc.init(&md);
	hash_desc.process(&md, (const unsigned char*)msg.c_str(), (unsigned long)msg.size());
	hash_desc.done(&md, hash);

	const int padding = LTC_PKCS_1_V1_5;
	const unsigned long saltlen = 0;

	int stat = 0;
	rsa_verify_hash_ex((const unsigned char*)sign.data(), sign.size(), hash, hash_desc.hashsize, padding, hash_idx, saltlen, &stat, const_cast<rsa_key *>(&this->_key));
	if (!stat) return false;

	return (true);
}

bool bkc::rsaKey::verifyPrintable(const std::string &msg, const std::string &sign) const
{
	std::string data;
	std::string tmp;
	std::string str = sign;

	tmp = blc::tools::serializable::cut(str, ' ');
	try {
		for(;tmp != ""; tmp = blc::tools::serializable::cut(str, ' '))
			data += (char)std::stoi(tmp);
	} catch (std::invalid_argument &e) {
		throw blc::error::exception(assertError(e.what()));
	}
	return (this->verify(msg, data));
}

std::string bkc::rsaKey::printablePub() const
{
	std::string key = this->getPub();
	std::string ret;

	for (auto it : key)
		ret += std::to_string((int)it) + " ";
	return (ret);
}

std::string bkc::rsaKey::printablePriv() const
{
	std::string key = this->getPriv();
	std::string ret;

	for (auto it : key)
		ret += std::to_string((int)it) + " ";
	return (ret);
}

void bkc::rsaKey::importPub(std::string str)
{
	std::string key;
	std::string tmp;

	tmp = blc::tools::serializable::cut(str, ' ');
	try {
		for(;tmp != ""; tmp = blc::tools::serializable::cut(str, ' '))
			key += (char)std::stoi(tmp);
	} catch (std::invalid_argument &e) {
		throw blc::error::exception(assertError(e.what()));
	}
	this->setPub(key);
}

void bkc::rsaKey::importPriv(std::string str)
{
	std::string key;
	std::string tmp;

	tmp = blc::tools::serializable::cut(str, ' ');
	try {
		for(;tmp != ""; tmp = blc::tools::serializable::cut(str, ' '))
			key += (char)std::stoi(tmp);
	} catch (std::invalid_argument &e) {
		throw blc::error::exception(assertError(e.what()));
	}
	this->setPriv(key);
}

bkc::rsaKey bkc::rsaKey::open(const std::string &pubFile, const std::string &privFile)
{
	std::ifstream	file;
	unsigned char	*buf;
	int		size;

	std::string pub_key = bkc::rsaKey::open(pubFile);
	std::string priv_key = bkc::rsaKey::open(privFile);

	bkc::rsaKey key(pub_key, priv_key);

	return (key);
}

bkc::rsaKey bkc::rsaKey::make()
{
	const int prng_idx = register_prng(&sprng_desc);
	if (prng_idx < 0) throw blc::error::exception(assertError("rng not loading"));

	rsa_key key;
	const int bitsize = 2048;
	int err = rsa_make_key(NULL, prng_idx, bitsize/8, 65537, &key);
	if (err != CRYPT_OK) throw blc::error::exception(assertError("key could not be generated"));

	return (bkc::rsaKey(key));
}

std::string bkc::rsaKey::open(const std::string &keyFile)
{
	std::ifstream	file(keyFile, std::ios::in|std::ios::binary|std::ios::ate);

	if (file.is_open() == false) throw blc::error::exception(assertError("opening of \"" + keyFile + "\" failed"));
	int size = blc::tools::fileSize(file);
	unsigned char *buf = new unsigned char [size];
	file.read((char *)buf, size);
	if (size < 1) throw blc::error::exception(assertError("reading of \"" + keyFile + "\" failed"));
	file.close();
	std::string key(buf, buf + size);
	delete [] buf;

	return (key);
}
