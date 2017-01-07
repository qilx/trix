#ifndef TRIX_DISPATCHER_HPP_
#define TRIX_DISPATCHER_HPP_

#include <boost/function.hpp>
#include <string>
#include <memory>

#include "connection.hpp"
#include "handler.hpp"
#include "request_parser.hpp"

namespace trix {

class Dispatcher {

	private:

		Handler & handler;
		RequestParser requestParser;

		void doDispatch(std::shared_ptr<Connection> connection, std::shared_ptr<std::string> data);
		void handleErrorResponse(const Request & request, Response & response, const ResponseException & ex);

	public:

		Dispatcher(Handler & handler);

		void dispatch(std::shared_ptr<Connection> connection, std::shared_ptr<std::string> data);

};

} /* namespace trix */

#endif /* TRIX_DISPATCHER_HPP_ */
