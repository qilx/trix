
#ifndef TRIX_COMMON_HPP_
#define TRIX_COMMON_HPP_

#include <map>
#include <string>

#include "utils/static_map.hpp"

namespace trix {

enum Method {
	UNKNOWN = 0,
    GET     = 1,
    POST    = 2,
	HEAD    = 4,
	PUT     = 8,
	DELETE  = 16,
	OPTIONS = 32,
	ALL     = 65535
};

class MethodsMap : public utils::StaticMap<MethodsMap, std::string, Method> {

	public:
		static void init();

};

class StatusCodesMap : public utils::StaticMap<StatusCodesMap, unsigned short, std::string> {

	public:
		static void init();

};

Method stringToMethod(const std::string & methodName);

} /* namespace trix */

#endif /* TRIX_COMMON_HPP_ */

