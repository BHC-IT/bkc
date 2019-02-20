#pragma once

#include <bfc/bfc.hpp>
#include <vector>
#include <string>


namespace bkc {
	const std::vector<std::string> yes({"yes", "Yes", "y", "Y", "YES"});
	const std::vector<std::string> no({"no", "No", "n", "N", "NO"});
	const bfc::color::modifier green(bfc::color::FG_GREEN);
	const bfc::color::modifier red(bfc::color::FG_RED);
	const bfc::color::modifier def(bfc::color::FG_DEFAULT);

	std::string initChain();
}
