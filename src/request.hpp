#ifndef TRIX_REQUEST_HPP_
#define TRIX_REQUEST_HPP_

#include <string>
#include <map>

#include "common.hpp"
#include "utils/querydict.hpp"

namespace trix {

class Request {

	private:

		friend class RequestParser;

		std::string remoteHostname;
		unsigned short remotePort = 0;

		std::string protocol;
		Method method;
		std::string rawUri;
		std::string uri;
		std::string query;
		std::string anchor;

		std::map<std::string, std::string> headers;

		std::map<std::string, std::string> uriParams;
		utils::QueryDict getParams;
		utils::QueryDict postParams;

		// cookies
		// files
		// get data
		// post data
		// uri data
		// input

	public:

		const std::string & getRemoteHostname() const;
		const unsigned short getRemotePort() const;
		const std::string & getProtocol() const;
		const Method & getMethod() const;
		const std::string & getRawUri() const;
		const std::string & getUri() const;
		const std::string & getQuery() const;
		const std::string & getAnchor() const;
		const std::map<std::string, std::string> & getHeaders() const;
		const std::map<std::string, std::string> & getUriParams() const;
		const utils::QueryDict & getGetParams() const;
		const utils::QueryDict & getPostParams() const;

};

} /* namespace trix */

#endif /* TRIX_REQUEST_HPP_ */
