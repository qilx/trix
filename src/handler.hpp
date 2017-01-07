#ifndef TRIX_HANDLER_HPP_
#define TRIX_HANDLER_HPP_

#include <boost/function.hpp>
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "common.hpp"
#include "connection.hpp"
#include "request.hpp"
#include "response.hpp"

namespace trix {

class Handler;

typedef boost::function<void(const Request&, Response&)> func_t;

struct RequestMapping;
struct RequestHandlerMapping;
struct RequestMappingResult;

class AbstractHandler {

	public:

		virtual void addMapping(const std::string & uri, const func_t & func) = 0;
		virtual void addMapping(const std::string & uri, const unsigned short & methods, const func_t & func) = 0;
		virtual void addHandlerMapping(const std::string & uri, const Handler & handler) = 0;

		//virtual ~AbstractHandler();

};

class Handler : AbstractHandler {

	private:

		std::vector<std::shared_ptr<RequestHandlerMapping>> handlerMappings;
		std::vector<std::shared_ptr<RequestMapping>> mappings;

		std::vector<std::string> getCaptureNamesFromRegex(const std::string & regex);
		std::shared_ptr<RequestMappingResult> findMapping(const std::string & uri, const Method & method);

	public:

		Handler();

		std::shared_ptr<RequestMappingResult> handle(const std::string & requestHeader);

		void addMapping(const std::string & uri, const func_t & func);
		void addMapping(const std::string & uri, const unsigned short & methods, const func_t & func);
		void addHandlerMapping(const std::string & uri, const Handler & handler);

};

class HandlerProxy : AbstractHandler {

	private:

		Handler handler;

	protected:

		Handler & getHandler();

	public:

		void addMapping(const std::string & uri, const func_t & func);
		void addMapping(const std::string & uri, const unsigned short & methods, const func_t & func);
		void addHandlerMapping(const std::string & uri, const Handler & handler);

};

struct RequestMapping {
	std::string uri;
	unsigned short methods;
	func_t func;
	boost::regex expr;
	std::vector<std::string> captureNames;
};

struct RequestHandlerMapping {
	std::string uri;
	Handler handler;
	boost::regex expr;
	std::vector<std::string> captureNames;
};

struct RequestMappingResult {
	func_t func;
	std::map<std::string, std::string> uriParams;
};


} /* namespace trix */

#endif /* TRIX_HANDLER_HPP_ */
