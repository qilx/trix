
#include "connection.hpp"
#include "connection_manager.hpp"

namespace trix {

Connection::Connection(ConnectionManager &connectionManager, boost::asio::io_service & ioService):
	connectionManager(connectionManager), socket(ioService) {
}

void Connection::receive(boost::asio::streambuf & buffer, std::size_t length) {

	std::size_t done = 0;

	if (this->streambuf.size() > 0) {

		std::size_t bytesCopied = boost::asio::buffer_copy(
			buffer.prepare(this->streambuf.size()),
			this->streambuf.data(),
			length
		);

		buffer.commit(bytesCopied);

		done += bytesCopied;

	}

	boost::system::error_code error;

	boost::asio::read(
		this->socket,
		buffer,
		boost::asio::transfer_exactly(length - buffer.size()),
		error
	);

}

bool Connection::isOpen() {
	return this->socket.is_open();
}

void Connection::close() {
	this->socket.close();
	this->connectionManager.remove(shared_from_this());
}

} /* namespace trix */
