
#include "request.hpp"

namespace trix {

Request::Request(): remotePort(0) {}

const std::string & Request::getRemoteHostname() const {
	return this->remoteHostname;
}

const unsigned short Request::getRemotePort() const {
	return this->remotePort;
}

const std::string & Request::getProtocol() const {
	return this->protocol;
}

const Method & Request::getMethod() const {
	return this->method;
}

const std::string & Request::getRawUri() const {
	return this->rawUri;
}

const std::string & Request::getUri() const {
	return this->uri;
}

const std::string & Request::getQuery() const {
	return this->query;
}

const std::string & Request::getAnchor() const {
	return this->anchor;
}

const std::map<std::string, std::string> & Request::getUriParams() const {
	return this->uriParams;
}

const std::map<std::string, std::string> & Request::getGetParams() const {
	return this->getParams;
}

} /* namespace trix */
