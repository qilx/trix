
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

		request.headers.push_back(std::make_pair(key, value));

	}

}

void RequestParser::parseGetParams(Request & request) {

	if (request.query.size() == 0) {
		return;
	}

	const std::string & query = request.query;

	std::size_t start = 0;
	std::size_t end = query.size();

	bool run = true;
	while (run) {

		std::size_t paramEnd = query.find("&", start);

		if (paramEnd == std::string::npos) {
			paramEnd = end;
			run = false;
		}

		std::size_t paramSeparator = query.find("=", start);
		if (paramSeparator == std::string::npos) {
			paramSeparator = end;
		}

		std::string key = query.substr(start, paramSeparator - start);
		key = utils::Uri::decode(key);

		std::string val;
		if (paramSeparator < paramEnd) {
			val = query.substr(paramSeparator + 1, paramEnd - paramSeparator - 1);
			val = utils::Uri::decode(val);
		}

		request.getParams[key] = val;

		start = paramEnd + 1;
	}

}

bool RequestParser::getline(std::string &data, std::string &line) {

	std::size_t i = data.find(LINE_DELIMITER);

	if (i == std::string::npos) {
		return false;
	}

	line = data.substr(0, i);
	data.erase(0, i + 2);

	return true;
}

} /* namespace trix */
