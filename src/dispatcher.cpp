
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <string>

#include "dispatcher.hpp"
#include "request_parser.hpp"
#include "response.hpp"

namespace trix {

Dispatcher::Dispatcher(Handler & handler) : handler(handler) {}

void Dispatcher::dispatch(std::shared_ptr<Connection> connection, std::shared_ptr<std::string> data) {

	boost::thread thread(boost::bind(&Dispatcher::doDispatch, this, connection, data));

}

void Dispatcher::doDispatch(std::shared_ptr<Connection> connection, std::shared_ptr<std::string> data) {

	std::shared_ptr<RequestMappingResult> mapping = this->handler.handle(*data);

	Request request = this->requestParser.parse(connection->getSocket(), *data, mapping);
	Response response(connection->getSocket(), request);

	try {

		try {

			if (!mapping) {
				throw ResponseException(404);
			}

			mapping->func(request, response);

		} catch (const ResponseException & ex) {
			throw ex;
		} catch (const std::exception & ex) {
			throw ResponseException(500, "", ex.what());
		} catch (const std::string & ex) {
			throw ResponseException(500, "", ex);
		} catch (...) {
			throw ResponseException(500);
		}

	} catch (const ResponseException & ex) {
		this->handleErrorResponse(request, response, ex);
	}

	if (!response.isWritten()) {
		response.writeHeaders();
		response.writeContent();
	}

	connection->close();

}

void Dispatcher::handleErrorResponse(const Request & request, Response & response, const ResponseException & ex) {

	response.setStatusCode(ex.getStatusCode());
	response.setStatusMessage(ex.getStatusMessage());

	std::string content =
			"<html>"
			"  <head>"
			"    <title>" + ex.getStatusMessage() + "</title>"
			"  </head>"
			"  <body>"
			"    <h1>" + ex.getStatusMessage() + "</h1>"
			"    <p>" + ex.getDetail() + "</p>"
			"  </body>"
			"</html>";

	response << content;

}

} /* namespace trix */
