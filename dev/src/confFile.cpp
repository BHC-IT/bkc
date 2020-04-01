#include <blc/blc.hpp>
#include <nlohmann/json.hpp>
#include <bfc/bfc.hpp>
#include <rsa/rsaKey.hpp>

#include "adminConnector.hpp"
#include "cinConnector.hpp"
#include "peerServer.hpp"
#include "identity.hpp"

#include "handleKeyParam.hpp"
#include "confFile.hpp"
#include "identity.hpp"

#include "trans.hpp"
#include "chain.hpp"
#include "lists.hpp"

#include <stdio.h>
#include <ios>
#include <iomanip>

bkc::rsaKey bkc::admKey;

static void launch(std::string addr, int port)
{
	if (bfc::flags::isSet("a")){
		std::cout << bfc::color::modifier(bfc::color::FG_GREEN) << "port : " << port << " is now listenning for incoming peer" << bfc::color::modifier(bfc::color::FG_DEFAULT) << std::endl;
		bfc::factory<bkc::node::peerServ>("server", 50, port);
	}
	else {
		bfc::factory<bkc::node::admCon>("adm", addr, port);
	}
	if (bfc::flags::isSet("input")){
		std::ifstream *i = new std::ifstream(bfc::flags::getValue("input"));
		bfc::factory<bkc::node::cinCon>("cin", *i);
	}
	bfc::factory<bkc::node::cinCon>("cin");
}

void readConfFile(const std::string &fileName)
{
	std::ifstream file;
	std::string tmp;
	int port;

	try {
		file.open(fileName);
		long file_size = blc::tools::fileSize(file);
		char *data = new char [file_size];
		file.read(data, file_size);
		file.close();
		tmp = std::string(data, data + file_size);
		delete [] data;
	} catch (blc::error::exception &e) {
		std::cerr << e.what() << std::endl;
		throw blc::error::exception(assertError("error with \"" + fileName + "\" file"));
	}

	nlohmann::json j;
	std::string addr;
	std::string in = "./dump.dc";
	std::string out = "./dump.dc";
	if (bfc::flags::isSet("output_chain"))
		out = bfc::flags::getValue("output_chain");
	if (bfc::flags::isSet("input_chain"))
		in = bfc::flags::getValue("input_chain");

	try {
		j = nlohmann::json::parse(tmp);
		addr = j["ip"];
		port = j["port"].get<int>();
		bkc::rsaKey admKey;

		if (j.count("whitelist") && j["whitelist"].get<bool>())
			bkc::lists::setWhiteList();
		else if (j.count("blacklist") && j["blacklist"].get<bool>())
			bkc::lists::setBlackList();

		admKey.importPub(j["adm_key"].get<std::string>());
		bkc::admKey = admKey;
		bfc::factory<bkc::chain>("chain", admKey, j["adm_lvl"].get<int>(), in, out);
	} catch (blc::error::exception &e) {
		std::cerr << e.what() << std::endl;
		throw blc::error::exception(assertError("error parsing conf file"));
	} catch (nlohmann::detail::parse_error &e) {
		std::cerr << e.what() << std::endl;
		throw blc::error::exception(assertError("error parsing conf file"));
	}

	try {
		launch(addr, port);
	} catch (blc::error::exception &e) {
		std::cerr << e.what() << std::endl;
		throw blc::error::exception(assertError("error in launch"));
	}
}