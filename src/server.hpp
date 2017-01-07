#ifndef TRIX_SERVER_HPP_
#define TRIX_SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <string>

#include "connection_manager.hpp"
#include "dispatcher.hpp"
#include "handler.hpp"

namespace trix {

class Server : public HandlerProxy {

	private:

		static const std::string DATA_SEPARATOR;

		long requestTimeout = 90L;
		std::size_t maxConnectionsLimit = 60;

		/// The io_service used to perform asynchronous operations.
		boost::asio::io_service ioService;

		/// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals;

		/// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor;

		ConnectionManager connectionManager;

		Dispatcher dispatcher;

		boost::mutex acceptMutex;
		bool acceptPaused = false;

		void doAccept();

		void doAwaitStop();

		void doRead(std::shared_ptr<Connection> connection);

		std::shared_ptr<boost::asio::deadline_timer> getTimeoutTimer(std::shared_ptr<Connection> connection);

		void onConnectionClose(std::shared_ptr<Connection> connection);

	public:

		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		explicit Server(const std::string& address, const std::string& port);

		void run();

		void setRequestTimeout(long requestTimeout);
		long getRequestTimeout();

		void setMaxConnectionsLimit(std::size_t limit);
		std::size_t getMaxConnectionsLimit();

};

} /* namespace trix */

#endif /* TRIX_SERVER_HPP_ */
