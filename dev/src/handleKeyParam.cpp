#include <blc/blc.hpp>
#include <nlohmann/json.hpp>
#include <bfc/bfc.hpp>
#include "adminConnector.hpp"
#include "cinConnector.hpp"
#include "peerServer.hpp"
#include "identity.hpp"

#include "rsaKey.hpp"

#include <stdio.h>
#include <ios>
#include <iomanip>

static bool createKey(std::string pub_name, std::string pri_name)
{
	bfc::color::modifier green(bfc::color::FG_GREEN);
	bfc::color::modifier def(bfc::color::FG_DEFAULT);

	std::cout << green << "creating key" << def << std::endl;
		bkc::rsaKey key = bkc::rsaKey::make();

		std::ofstream pub(pub_name);
		std::ofstream pri(pri_name);

		std::cout << green << "public key file : " << def << pub_name << std::endl;
		std::cout << green << "private key file : " << def << pri_name << std::endl;
		std::cout << green << "keep it safe and sound. don't share it. it make you able to authentificate." << def << std::endl;

		pub << key.getPub();
		pri << key.getPriv();

		if (bfc::flags::isSet("use")){
			bkc::myLog = key;
		} else {
			bfc::exit();
			return (false);
		}
		return (true);
}

bool handleKey()
{
	std::string pub_name = "public_key.der";
	std::string pri_name = "private_key.der";

	if (bfc::flags::isSet("pub"))
		pub_name = bfc::flags::getValue("pub");
	if (bfc::flags::isSet("pri"))
		pri_name = bfc::flags::getValue("pri");

	if (bfc::flags::isSet("create")) {
		return (createKey(pub_name, pri_name));
	} else {
		bkc::myLog = bkc::rsaKey::open(pub_name, pri_name);
	}
	return (true);
}