#ifndef TRIX_REQUEST_PARSER_HPP_
#define TRIX_REQUEST_PARSER_HPP_

#include <boost/asio.hpp>
#include <string>
#include <map>
#include <memory>

#include "handler.hpp"
#include "request.hpp"

namespace trix {

class RequestParser {

	private:
		static const std::string LINE_DELIMITER;

		void parseRemoteData(Request & request, std::shared_ptr<Connection> connection);
		void parseRequestData(Request & request, std::string & data);
		void parseHeaders(Request & request, std::string & data);
		void parseGetParams(Request & request);
		bool getline(std::string &data, std::string &line);

	public:

		//RequestParser(const RequestParser&) = delete;
		//RequestParser& operator=(const RequestParser&) = delete;

		RequestParser();

		Request parse(std::shared_ptr<Connection> connection, std::string data, std::shared_ptr<RequestMappingResult> mapping);

};

} /* namespace trix */

#endif /* TRIX_REQUEST_PARSER_HPP_ */
