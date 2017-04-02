
#ifndef TRIX_CONNECTION_HPP_
#define TRIX_CONNECTION_HPP_

#include <boost/asio.hpp>
#include <memory>

namespace trix {

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {

private:

	ConnectionManager &connectionManager;


public:

	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf streambuf;

	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	Connection(ConnectionManager & connectionManager, boost::asio::io_service & ioService);

	void receive(boost::asio::streambuf & streambuf, std::size_t length);

	bool isOpen();
	void close();

};

} /* namespace trix */

#endif /* TRIX_CONNECTION_HPP_ */
