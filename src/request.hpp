#ifndef TRIX_REQUEST_HPP_
#define TRIX_REQUEST_HPP_

#include <string>
#include <list>
#include <map>

#include "common.hpp"

namespace trix {

class Request {

	private:

		friend class RequestParser;

		std::string remoteHostname;
		unsigned short remotePort;

		std::string protocol;
		Method method;
		std::string rawUri;
		std::string uri;
		std::string query;
		std::string anchor;

		std::list<std::pair<std::string, std::string>> headers;

		std::map<std::string, std::string> getParams;
		std::map<std::string, std::string> uriParams;
		// cookies
		// files
		// get data
		// post data
		// uri data
		// input

	public:

		//Request(const Request&) = delete;
		//Request& operator=(const Request&) = delete;

		Request();

		const std::string & getRemoteHostname() const;
		const unsigned short getRemotePort() const;
		const std::string & getProtocol() const;
		const Method & getMethod() const;
		const std::string & getRawUri() const;
		const std::string & getUri() const;
		const std::string & getQuery() const;
		const std::string & getAnchor() const;
		const std::map<std::string, std::string> & getUriParams() const;
		const std::map<std::string, std::string> & getGetParams() const;

};

} /* namespace trix */

#endif /* TRIX_REQUEST_HPP_ */
