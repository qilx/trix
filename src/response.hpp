#ifndef TRIX_RESPONSE_HPP_
#define TRIX_RESPONSE_HPP_

#include <boost/asio.hpp>
#include <string>
#include <list>
#include <memory>

#include "request.hpp"

namespace trix {

class Response {

	private:

		friend class Dispatcher;

		bool written = false;
		boost::asio::ip::tcp::socket & socket;

		// --

		std::string protocol;
		unsigned short statusCode = 200;
		std::string statusMessage = "OK";

		std::string contentType = "text/html";

		long contentLength = -1;
		boost::asio::streambuf content;

		std::list<std::pair<std::string, std::string>> headers;

		void writeHeaders();
		void writeContent();

	public:

		enum statusTypes {
		    ok = 200,
		    created = 201,
		    accepted = 202,
		    no_content = 204,
		    multiple_choices = 300,
		    moved_permanently = 301,
		    moved_temporarily = 302,
		    not_modified = 304,
		    bad_request = 400,
		    unauthorized = 401,
		    forbidden = 403,
		    not_found = 404,
		    internal_server_error = 500,
		    not_implemented = 501,
		    bad_gateway = 502,
		    service_unavailable = 503
		};


		Response(boost::asio::ip::tcp::socket & socket, Request & request);

		void setStatusCode(unsigned short statusCode);
		void setStatusMessage(const std::string & statusMessage);
		void setContentType(const std::string & contentType);
		void setContentLength(long contentLength);

		void addHeader(const std::string, const std::string);

		Response & operator<<(const std::string & data);
		void write(const char * data, const std::size_t length);

		bool isWritten();

};

class ResponseException {

	private:

		unsigned short statusCode;
		std::string statusMessage;
		std::string detail;

		std::string handleStatusMessage(const unsigned short & statusCode, const std::string & msg);

	public:

		ResponseException(const unsigned short & statusCode);
		ResponseException(const unsigned short & statusCode, const std::string & statusMessage);
		ResponseException(const unsigned short & statusCode, const std::string & statusMessage, const std::string & detail);

		const unsigned short & getStatusCode() const;
		const std::string & getStatusMessage() const;
		const std::string & getDetail() const;

};

} /* namespace trix */

#endif /* TRIX_RESPONSE_HPP_ */
