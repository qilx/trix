#ifndef TRIX_CONNECTION_MANAGER_HPP_
#define TRIX_CONNECTION_MANAGER_HPP_

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <set>

#include "connection.hpp"

namespace trix {

class ConnectionManager {

	private:

		boost::mutex mutex;

		std::set<std::shared_ptr<Connection>> connections;
		boost::function<void(std::shared_ptr<Connection>)> onRemoveHandler;

	public:

		ConnectionManager();

		void add(std::shared_ptr<Connection> c);
		void remove(std::shared_ptr<Connection> c);
		std::size_t size();
		void close();

		void setOnRemoveHandler(boost::function<void(std::shared_ptr<Connection>)> onRemoveHandler);

};

} /* namespace trix */

#endif /* TRIX_CONNECTION_MANAGER_HPP_ */
