
#ifndef TRIX_UTILS_URI_HPP_
#define TRIX_UTILS_URI_HPP_

#include <string>

namespace trix {
namespace utils {

class Uri {

	public:
		Uri(const Uri&) = delete;
		Uri& operator=(const Uri&) = delete;

		static std::string encode(const std::string & uri);
		static std::string decode(const std::string & uri);
};


} /* namespace uri */
} /* namespace trix */

#endif /* TRIX_UTILS_URI_HPP_ */
