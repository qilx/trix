
#ifndef TRIX_UTILS_STATIC_MAP_HPP_
#define TRIX_UTILS_STATIC_MAP_HPP_

#include <map>

namespace trix {
namespace utils {

template<class T, class K, class V>
class StaticMap {

	private:

		static StaticMap<T,K,V> _instance;
		std::map<K,V> data;

		StaticMap() {
			T::init();
		}

	protected:

		static void put(const K & key, const V & value) {
			_instance.data[key] = value;
		}

	public:

		static bool has(const K & key) {

			auto it = _instance.data.find(key);
			if(it != _instance.data.end()) {
				return true;
			}

			return false;
		}

		static V get(const K & key) {
			return _instance.data[key];
		}

};

template<class T, class K, class V>
StaticMap<T,K,V> StaticMap<T,K,V>::_instance;

} /* namespace utils */
} /* namespace trix */

#endif /* TRIX_UTILS_STATIC_MAP_HPP_ */
