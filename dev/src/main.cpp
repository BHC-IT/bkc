#include <blc/blc.hpp>
#include <nlohmann/json.hpp>
#include <bfc/bfc.hpp>
#include "adminConnector.hpp"
#include "cinConnector.hpp"
#include "peerServer.hpp"
#include "identity.hpp"

#include "handleKeyParam.hpp"
#include "confFile.hpp"
#include "initChain.hpp"

#include "rsaKey.hpp"
#include "getMyIp.hpp"

#include "lists.hpp"

#include "trans.hpp"
#include "chain.hpp"

#include <stdio.h>
#include <ios>
#include <iomanip>

bkc::rsaKey	bkc::myLog;
std::string	bkc::ip;
std::string	bkc::url;
int		bkc::port;

void bfc::initActor()
{
	ltc_mp = ltm_desc;

	bfc::usage.add({"--help"}, "show this helper");
	bfc::usage.add({"--create"}, "create a pair of key. will erase already existing key");
	bfc::usage.add({"--conf=FILE"}, "chain conf file to use. default is ./conf.bkc");
	bfc::usage.add({"--init=CHAIN_NAME"}, "create a init file for a new chain");
	bfc::usage.add({"--use"}, "use the newly created key and/or configuration file to run");
	bfc::usage.add({"--a"}, "run as admin of the chain");
	bfc::usage.add({"--input=FILE"}, "read a file as the new stdin");
	bfc::usage.add({"--output_chain=FILE"}, "file to dump the chain state, default is ./dump.dc");
	bfc::usage.add({"--input_chain=FILE"}, "file to get the chain state, default is ./dump.dc");
	bfc::usage.add({"--pub=FILE"}, "select the public key file to read (or write if --create is used)");
	bfc::usage.add({"--pri=FILE"}, "select the public key file to read (or write if --create is used)");
	bfc::usage.add({"--conf=FILE"}, "read a designated conf file. default: ./conf.bkc");
	bfc::usage.add({"--version"}, "print version");
	bfc::usage.add({"--list=FILE"}, "select a file to read to white/black list from");

	if (bfc::flags::isSet("help")){
		std::cout << bfc::usage << std::endl;
		bfc::exit();
		return;
	}
	if (bfc::flags::isSet("version")){
		std::cout << "\tbkc version : " << BKC_MAJOR_VERSION << "." << BKC_MINOR_VERSION << ". (build:" << BKC_PATCH_VERSION << ")" << std::endl;
		std::cout << "\trunning with bfc version : " << BFC_MAJOR_VERSION << "." << BFC_MINOR_VERSION << std::endl;
		bfc::exit();
		return;
	}
	if (handleKey() == false)
		return;
	std::string file = "conf.bkc";
	if (bfc::flags::isSet("init")){
		if ((bkc::initChain() = file) == "")
			return;
	}
	if (bfc::flags::isSet("conf")) {
		readConfFile(bfc::flags::getValue("conf"));
	} else {
		readConfFile(file);
	}
	if (bfc::flags::isSet("list")){
		bkc::lists::load(bfc::flags::getValue("list"));
	} else {
		bkc::lists::load("list.bkc");
	}
}

int bfc::main()
{
	try {
		if (bfc::flags::isSet("a") == false) {
			int port = blc::network::findFreePort();

			bkc::port = port;
			bkc::ip = bkc::getMyIp();
			bkc::url = bkc::ip + ":" + std::to_string(bkc::port);
			std::cout << port << std::endl;

			bfc::factory<bkc::node::peerServ>("server", 50, port);
		}
	} catch (blc::error::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}


void bfc::closure()
{
	bkc::chain	*chain = dynamic_cast<bkc::chain *>(bfc::masterThread::rep("chain"));

	chain->dump();
}

void bfc::masterThread::readActor()
{
	if (bfc::flags::isSet("a") == false)
		this->for_each({"adm"}, this->_adm);
	this->for_each({"cin"}, this->_cin);
	this->for_each({"server"}, this->_serv);
	this->for_each({"peer*"}, this->_peer);
	this->for_each({"chain"}, this->_chain);
}

void bfc::masterThread::thick()
{
	static int i = 0;

	if (i == 0){
		if (bfc::flags::isSet("a") == false)
			this->adminProto();
		this->cinProto();
		this->peerProto();
		this->servProto();
		this->chainProto();
	}

	i++;
	if (i == 200000 && bfc::flags::isSet("a") == false){
		this->masterPipe["adm"] << "401" << blc::endl << blc::endl;
		i = 0;
	}
}
