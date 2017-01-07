
#include "connection.hpp"
#include "connection_manager.hpp"

namespace trix {

Connection::Connection(ConnectionManager &connectionManager, boost::asio::io_service & ioService):
//Connection::Connection(ConnectionManager &connectionManager, std::shared_ptr<boost::asio::ip::tcp::socket> socket):
	connectionManager(connectionManager), socket(ioService) {
}

//std::shared_ptr<boost::asio::ip::tcp::socket> Connection::getSocket() {
boost::asio::ip::tcp::socket & Connection::getSocket() {
	return this->socket;
}

bool Connection::isOpen() {
	return this->socket.is_open();
}

void Connection::close() {
	this->socket.close();
	this->connectionManager.remove(shared_from_this());
}

} /* namespace trix */
