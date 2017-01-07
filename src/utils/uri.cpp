
#include "uri.hpp"

namespace trix {
namespace utils {

    std::string Uri::encode(const std::string & uri) {

        //RFC 3986 section 2.3 Unreserved Characters (January 2005)
        const std::string unreserved = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

        std::string escaped = "";

        for(size_t i=0; i<uri.length(); i++) {
            if (unreserved.find_first_of(uri[i]) != std::string::npos) {
                escaped.push_back(uri[i]);
            } else {
                escaped.append("%");
                char buf[3];
                sprintf(buf, "%.2X", uri[i]);
                escaped.append(buf);
            }
        }

        return escaped;
    }

    std::string Uri::decode(const std::string & uri) {

        std::string ret;
        char ch;
        int i, j, len = uri.length();

        for (i=0; i < len; i++) {
            if (uri[i] != '%') {
                if (uri[i] == '+') {
                    ret += ' ';
                } else {
                    ret += uri[i];
                }
            } else {
                sscanf(uri.substr(i + 1, 2).c_str(), "%x", &j);
                ch = static_cast<char>(j);
                ret += ch;
                i = i + 2;
            }
        }

        return ret;
    }

} /* namespace uri */
} /* namespace trix */
