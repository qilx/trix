
#include "connection.hpp"
#include "connection_manager.hpp"

namespace trix {

Connection::Connection(ConnectionManager &connectionManager, boost::asio::io_service & ioService):
	connectionManager(connectionManager), socket(ioService) {
}


bool Connection::isOpen() {
	return this->socket.is_open();
}

void Connection::close() {
	this->socket.close();
	this->connectionManager.remove(shared_from_this());
}

} /* namespace trix */
