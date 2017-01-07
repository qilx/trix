
#include <string>
#include <iostream>
#include "catch.hpp"
#include "../uri.hpp"

TEST_CASE ("Uri encoding/decoding", "[uri]") {

    std::string uri = "/%-test/huhu/()/";
    std::string encoded = trix::uri::encode(uri);
    std::string decoded = trix::uri::decode(encoded);

    REQUIRE(decoded == uri);

}
