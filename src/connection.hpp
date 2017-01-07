
#ifndef TRIX_CONNECTION_HPP_
#define TRIX_CONNECTION_HPP_

#include <boost/asio.hpp>
#include <memory>

namespace trix {

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {

private:

	ConnectionManager &connectionManager;
	//std::shared_ptr<boost::asio::ip::tcp::socket> socket;
	boost::asio::ip::tcp::socket socket;

public:

	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	//Connection(ConnectionManager &connectionManager, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	Connection(ConnectionManager & connectionManager, boost::asio::io_service & ioService);

	//std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();
	boost::asio::ip::tcp::socket & getSocket();

	bool isOpen();
	void close();

};

} /* namespace trix */

#endif /* TRIX_CONNECTION_HPP_ */
