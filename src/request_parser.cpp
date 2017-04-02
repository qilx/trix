
#include <iostream>
#include <string>
#include <sstream>

#include "common.hpp"
#include "request_parser.hpp"
#include "response.hpp"
#include "utils/uri.hpp"

namespace trix {

const std::string RequestParser::LINE_DELIMITER = "\r\n";

RequestParser::RequestParser() {}

Request RequestParser::parse(std::shared_ptr<Connection> connection, std::string data, std::shared_ptr<RequestMappingResult> mapping) {

	Request request;

	if (mapping) {
		request.uriParams = mapping->uriParams;
	}

	this->parseRemoteData(request, connection);
	this->parseRequestData(request, data);
	this->parseHeaders(request, data);
	this->parseGetParams(request);
	this->parsePostParams(request, connection);

	return request;
}

void RequestParser::parseRemoteData(Request & request, std::shared_ptr<Connection> connection) {

	std::string remoteHostname = connection->socket.lowest_layer().remote_endpoint().address().to_string();
	unsigned short remotePort = connection->socket.lowest_layer().remote_endpoint().port();

	request.remoteHostname = remoteHostname;
	request.remotePort     = remotePort;

}

void RequestParser::parseRequestData(Request & request, std::string & data) {

	std::string line;

	this->getline(data, line);

	std::size_t i1 = line.find(' ');
	std::size_t i2 = line.find(' ', i1 + 1);

	request.method   = stringToMethod(line.substr(0, i1));
	request.rawUri   = line.substr(i1 + 1, i2 -i1 - 1);
	request.protocol = line.substr(i2 + 1);

	std::size_t queryEnd = request.rawUri.size();
	std::size_t uriEnd   = request.rawUri.size();

	std::size_t anchorStart = request.rawUri.rfind("#");
	if (anchorStart != std::string::npos) {
		request.anchor = request.rawUri.substr(anchorStart + 1, request.rawUri.size() - anchorStart);
		queryEnd = anchorStart;
		uriEnd = anchorStart;
	}

	std::size_t queryStart = request.rawUri.find("?");
	if (queryStart != std::string::npos) {
		request.query = request.rawUri.substr(queryStart + 1, queryEnd - queryStart);
		uriEnd = queryStart;
	}

	request.uri = utils::Uri::decode(request.rawUri.substr(0, uriEnd));

}

void RequestParser::parseHeaders(Request & request, std::string & data) {

	std::string line;

	while (this->getline(data, line) && !line.empty()) {

		std::size_t keyEnd = line.find(":");
		std::size_t valueStart = keyEnd;

		if (line[keyEnd + 1] == ' ') {
			valueStart++;
		}

		std::string key = line.substr(0, keyEnd);
		std::string value = line.substr(valueStart + 1);

		request.headers[key] = value;

	}

}

void RequestParser::parseGetParams(Request & request) {

	if (request.query.size() == 0) {
		return;
	}

	request.getParams.load(request.query);

}

void RequestParser::parsePostParams(Request & request, std::shared_ptr<Connection> connection) {

	/*
	Content-Type: application/x-www-form-urlencoded
	Content-Length: 13
	*/

	if (request.headers.find("Content-Type") == request.headers.end()) {
		return;
	}

	if (request.headers["Content-Type"].compare("application/x-www-form-urlencoded") != 0) {
		return;
	}

	if (request.headers.find("Content-Length") == request.headers.end()) {
		return;
	}

	std::size_t contentLength;

	std::stringstream contentLengthStream(request.headers["Content-Length"]);
	contentLengthStream >> contentLength;

	if (contentLength <= 0) {
		return;
	}

	boost::asio::streambuf streambuf;

	connection->receive(streambuf, contentLength); // blocking call

	std::ostringstream ss;
	ss << &streambuf;
	std::string data = ss.str();

	request.postParams.load(data);

}

bool RequestParser::getline(std::string &data, std::string &line) {

	std::size_t i = data.find(LINE_DELIMITER);

	if (i == std::string::npos) {
		line = data.substr(0);
		data.erase(0);
		return true;
	}

	line = data.substr(0, i);
	data.erase(0, i + 2);

	return true;
}

} /* namespace trix */
