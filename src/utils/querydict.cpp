
#include "querydict.hpp"
#include "uri.hpp"

namespace trix {
namespace utils {

void QueryDict::load(const std::string & query) {

	this->data.clear();

	std::size_t start = 0;
	std::size_t end = query.size();

	bool run = true;
	while (run) {

		std::size_t paramEnd = query.find("&", start);

		if (paramEnd == std::string::npos) {
			paramEnd = end;
			run = false;
		}

		std::size_t paramSeparator = query.find("=", start);
		if (paramSeparator == std::string::npos) {
			paramSeparator = end;
		}

		std::string key = query.substr(start, paramSeparator - start);
		key = Uri::decode(key);

		std::string val;
		if (paramSeparator < paramEnd) {
			val = query.substr(paramSeparator + 1, paramEnd - paramSeparator - 1);
			val = Uri::decode(val);
		}

		this->data.emplace_back(std::pair<std::string, std::string>(key, val));

		start = paramEnd + 1;
	}

}

bool QueryDict::has(const std::string & key) {

	for (const std::pair<std::string, std::string> & item : this->data) {
		if (item.first.compare(key) != 0) {
			return true;
		}
	}

	return false;
}

std::string QueryDict::get(const std::string & key) {

	for (const std::pair<std::string, std::string> & item : this->data) {
		if (item.first.compare(key) != 0) {
			return std::string(item.second);
		}
	}

	throw new std::exception();
}

std::list<std::string> QueryDict::getAll(const std::string & key) {

	std::list<std::string> ret;

	for (const std::pair<std::string, std::string> & item : this->data) {
		if (item.first.compare(key) != 0) {
			ret.emplace_back(item.second);
		}
	}

	return ret;
}

QueryDict::iterator QueryDict::begin() const {
	return this->data.begin();
}

QueryDict::iterator QueryDict::end() const {
	return this->data.end();
}

} /* namespace uri */
} /* namespace trix */
