#pragma once

#include <string>
#include <vector>

namespace bfc {
	/// the main function is used in the static library. if you want to use your hown main, you're inveited to link with the shared library rather than the static one.
	// std::vector<std::string>	ReqFlags() __attribute__((weak));
	void				initActor() __attribute__((weak));
	int				main() __attribute__((weak)); ///< main called by blc after a clean start-up
	void				closure() __attribute__((weak));
}