
#ifndef TRIX_UTILS_QUERYDICT_HPP_
#define TRIX_UTILS_QUERYDICT_HPP_

#include <list>
#include <string>

namespace trix {
namespace utils {

class QueryDict {

	private:

		std::list<std::pair<std::string, std::string>> data;

	public:

		void load(const std::string & query);
		bool has(const std::string & key);
		std::string get(const std::string & key);
		std::list<std::string> getAll(const std::string & key);

		typedef std::list<std::pair<std::string, std::string>>::const_iterator iterator;

		iterator begin() const;
		iterator end() const;

};


} /* namespace uri */
} /* namespace trix */

#endif /* TRIX_UTILS_QUERYDICT_HPP_ */
