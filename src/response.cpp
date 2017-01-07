
#include "response.hpp"

namespace trix {

Response::Response(boost::asio::ip::tcp::socket & socket, Request & request) :
	socket(socket), protocol(request.getProtocol()) {
}

void Response::setStatusCode(unsigned short statusCode) {
	this->statusCode = statusCode;
}

void Response::setStatusMessage(const std::string & statusMessage) {
	this->statusMessage = statusMessage;
}

void Response::setContentType(const std::string & contentType) {
	this->contentType = contentType;
}

void Response::setContentLength(long contentLength) {
	this->contentLength = contentLength;
}

void Response::addHeader(const std::string name, const std::string value) {
	this->headers.push_back(std::make_pair(name, value));
}

void Response::write(const char * data, const std::size_t length) {

	if (this->contentLength == -1) {

		std::ostream os(&this->content);
		os.write(data, length);

	} else {

		if (!this->written) {
			this->writeHeaders();
		}

		boost::asio::streambuf streambuf;
		std::ostream os(&streambuf);

		os.write(data, length);

		this->socket.send(streambuf.data());
	}

}

void Response::writeHeaders() {

	this->written = true;

	long contentLength = this->contentLength;
	if (contentLength == -1) {
		contentLength = this->content.size();
	}

	boost::asio::streambuf streambuf;
	std::ostream os(&streambuf);

	os << this->protocol << " " << this->statusCode << " " << this->statusMessage << "\r\n";

	os << "Content-Type: " << this->contentType << "\r\n";
	os << "Content-Length: " << contentLength << "\r\n";

	for (std::pair<std::string, std::string> header : this->headers) {
		os << header.first << ": " << header.second << "\r\n";
	}

	os << "\r\n";

	this->socket.send(streambuf.data());

}

void Response::writeContent() {

	if (this->content.size() > 0 && this->contentLength == -1) {
		this->socket.send(this->content.data());
	}

}

Response & Response::operator<<(const std::string & data) {

	this->write(data.c_str(), data.size());

	return *this;
}

bool Response::isWritten() {
	return this->written;
}

/*************************************************************/

ResponseException::ResponseException(const unsigned short & statusCode):
	statusCode(statusCode), statusMessage(StatusCodesMap::get(statusCode)), detail() {}

ResponseException::ResponseException(const unsigned short & statusCode, const std::string & statusMessage):
	statusCode(statusCode), statusMessage(this->handleStatusMessage(statusCode, statusMessage)), detail() {}

ResponseException::ResponseException(const unsigned short & statusCode, const std::string & statusMessage, const std::string & detail):
	statusCode(statusCode), statusMessage(this->handleStatusMessage(statusCode, statusMessage)), detail(detail) {}

const unsigned short & ResponseException::getStatusCode() const {
	return this->statusCode;
}

const std::string & ResponseException::getStatusMessage() const {
	return this->statusMessage;
}

const std::string & ResponseException::getDetail() const {
	return this->detail;
}

std::string ResponseException::handleStatusMessage(const unsigned short & statusCode, const std::string & msg) {
	return msg == "" ? StatusCodesMap::get(statusCode) : msg;
}


} /* namespace trix*/
