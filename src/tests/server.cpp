
#include <string>
#include <iostream>
#include "catch.hpp"
#include "../server.hpp"

TEST_CASE ("Server", "[startup]") {

	using namespace trix::server;
	Server server("0.0.0.0", "8083", ".");
	server.run();

    REQUIRE(1 == 1);

}
