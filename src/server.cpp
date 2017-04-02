
#include <boost/bind.hpp>
#include <iostream>
#include <signal.h>
#include <utility>
#include <memory>

#include "server.hpp"
#include "connection.hpp"
#include "request.hpp"
#include "request_parser.hpp"

namespace trix {

const std::string Server::DATA_SEPARATOR = "\r\n\r\n";

Server::Server(const std::string& address, const std::string& port):
	ioService(), signals(ioService), acceptor(ioService), dispatcher(this->getHandler()) { //, connectionManager(ConnectionManager()),
	//dispatcher(Dispatcher()) {

	this->connectionManager.setOnRemoveHandler(boost::bind(&Server::onConnectionClose, this, _1));

	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	this->signals.add(SIGINT);
	this->signals.add(SIGTERM);
	#if defined(SIGQUIT)
	this->signals.add(SIGQUIT);
	#endif // defined(SIGQUIT)

	this->doAwaitStop();


	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(this->ioService);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});

	this->acceptor.open(endpoint.protocol());
	this->acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	this->acceptor.bind(endpoint);
	this->acceptor.listen();

	this->doAccept();

}

void Server::run() {
	this->ioService.run();
}

void Server::doAccept() {

	{
		boost::lock_guard<boost::mutex> guard(this->acceptMutex);

		if (this->connectionManager.size() >= this->maxConnectionsLimit - 1) {
			this->acceptPaused = true;
			return;
		}

	}

	//auto socket = std::make_shared<boost::asio::ip::tcp::socket>(this->ioService);
	std::shared_ptr<Connection> connection = std::make_shared<Connection>(this->connectionManager, this->ioService);

	this->acceptor.async_accept(connection->socket,
	    [this, connection](boost::system::error_code ec)
	    {
			// Check whether the server was stopped by a signal before this
	    	// completion handler had a chance to run.
	        if (!this->acceptor.is_open()) {
	        	return;
	        }

	        if (!ec) {

	        	this->connectionManager.add(connection);
	        	this->doRead(connection);

	        }

	        this->doAccept();
	    }
	);

}

void Server::doRead(std::shared_ptr<Connection> connection) {

	//Set timeout on the following boost::asio::async-read or write function
	std::shared_ptr<boost::asio::deadline_timer> timer = this->getTimeoutTimer(connection);

	boost::asio::async_read_until(connection->socket, connection->streambuf, DATA_SEPARATOR,
	    [this, timer, connection](const boost::system::error_code& ec, size_t bytes_transferred) {

			if(timer) {
	            timer->cancel();
	        }

	        if(!ec) {

	        	const char* data = boost::asio::buffer_cast<const char*>(connection->streambuf.data());

	        	std::shared_ptr<std::string> headers = std::make_shared<std::string>(data, bytes_transferred - DATA_SEPARATOR.length());
	        	connection->streambuf.consume(bytes_transferred);

	        	this->dispatcher.dispatch(connection, headers);

	        }

	    }
	);

}

std::shared_ptr<boost::asio::deadline_timer> Server::getTimeoutTimer(std::shared_ptr<Connection> connection) {

	if(this->requestTimeout == 0) {
		return nullptr;
	}

	std::shared_ptr<boost::asio::deadline_timer> timer = std::make_shared<boost::asio::deadline_timer>(this->ioService);
	timer->expires_from_now(boost::posix_time::seconds(this->requestTimeout));
	timer->async_wait([connection](const boost::system::error_code& ec){
		if(!ec) {
			connection->close();
		}
    });

    return timer;
}

void Server::onConnectionClose(std::shared_ptr<Connection> connection) {

	boost::lock_guard<boost::mutex> guard(this->acceptMutex);

	if (!this->acceptPaused) {
		return;
	}

	this->acceptPaused = false;
	this->doAccept();

}

void Server::doAwaitStop() {

	this->signals.async_wait(
	    [this](boost::system::error_code /*ec*/, int /*signo*/)
	    {
	    	// The server is stopped by cancelling all outstanding asynchronous
	    	// operations. Once all operations have finished the io_service::run()
	    	// call will exit.
	    	this->acceptor.close();
	    	this->connectionManager.close();
	    }
	);

}

void Server::setRequestTimeout(long requestTimeout) {
	this->requestTimeout = requestTimeout;
}
long Server::getRequestTimeout() {
	return this->requestTimeout;
}

void Server::setMaxConnectionsLimit(std::size_t limit) {
	this->maxConnectionsLimit = limit;
}

std::size_t Server::getMaxConnectionsLimit() {
	return this->maxConnectionsLimit;
}

} /* namespace trix */
