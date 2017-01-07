
#include "connection_manager.hpp"

namespace trix {

ConnectionManager::ConnectionManager() {}

void ConnectionManager::add(std::shared_ptr<Connection> c) {
	boost::lock_guard<boost::mutex> guard(this->mutex);
	this->connections.insert(c);
}

void ConnectionManager::remove(std::shared_ptr<Connection> c) {
	boost::lock_guard<boost::mutex> guard(this->mutex);
	this->connections.erase(c);
	if (this->onRemoveHandler) {
		this->onRemoveHandler(c);
	}
}

void ConnectionManager::close() {
	boost::lock_guard<boost::mutex> guard(this->mutex);
	for (std::shared_ptr<Connection> c : this->connections) {
		c->close();
	}
}

std::size_t ConnectionManager::size() {
	boost::lock_guard<boost::mutex> guard(this->mutex);
	return this->connections.size();
}

void ConnectionManager::setOnRemoveHandler(boost::function<void(std::shared_ptr<Connection>)> onRemoveHandler) {
	this->onRemoveHandler = onRemoveHandler;
}

} /* namespace trix */
