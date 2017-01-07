
#include "handler.hpp"
#include <iostream>

namespace trix {

Handler::Handler() {}

std::shared_ptr<RequestMappingResult> Handler::handle(const std::string & requestHeader) {

	std::size_t lineLength = requestHeader.find("\r\n");
	std::size_t methodStart = 0;
	std::size_t methodLength = requestHeader.find(" ", 0);

	std::size_t uriStart = methodStart + methodLength + 1;
	std::size_t uriLength = requestHeader.find(" ", uriStart) - methodLength - 1;

	std::size_t uriEnd;

	uriEnd = requestHeader.find("#", uriStart);
	if (uriEnd != std::string::npos) {
		uriLength = uriEnd - methodLength - 1;
	}

	uriEnd = requestHeader.find("?", uriStart);
	if (uriEnd != std::string::npos) {
		uriLength = uriEnd - methodLength - 1;
	}

	Method method = stringToMethod(requestHeader.substr(methodStart, methodLength));
	std::string uri = requestHeader.substr(uriStart, uriLength);

	return this->findMapping(uri, method);
}

std::shared_ptr<RequestMappingResult> Handler::findMapping(const std::string & uri, const Method & method) {

	for (const std::shared_ptr<RequestHandlerMapping> & handlerMapping : this->handlerMappings) {

		boost::smatch match;
		if (!boost::regex_search(uri, match, handlerMapping->expr)) {
			continue;
		}

		std::shared_ptr<RequestMappingResult> mappingResult = handlerMapping->handler.findMapping(match["TRIX_URI_REST"], method);
		if (!mappingResult) {
			continue;
		}

		for (const std::string name : handlerMapping->captureNames) {
			mappingResult->uriParams[name] = match[name];
		}

		return mappingResult;
	}

	for (const std::shared_ptr<RequestMapping> & mapping : this->mappings) {

		if ((mapping->methods & method) == Method::UNKNOWN) {
			continue;
		}

		boost::smatch match;
		if (!boost::regex_search(uri, match, mapping->expr)) {
			continue;
		}

		std::shared_ptr<RequestMappingResult> mappingResult = std::make_shared<RequestMappingResult>();
		mappingResult->func = mapping->func;

		for (const std::string name : mapping->captureNames) {
			mappingResult->uriParams[name] = match[name];
		}

		return mappingResult;
	}

	return nullptr;
}

void Handler::addMapping(const std::string & uri, const func_t & func) {
	this->addMapping(uri, Method::ALL, func);
}

void Handler::addMapping(const std::string & uri, const unsigned short & methods, const func_t & func) {

	std::shared_ptr<RequestMapping> mapping = std::make_shared<RequestMapping>();
	mapping->uri = uri;
	mapping->methods = methods;
	mapping->expr = boost::regex("^" + uri + "$");
	mapping->func = func;
	mapping->captureNames = this->getCaptureNamesFromRegex(uri);

	this->mappings.push_back(mapping);
}

void Handler::addHandlerMapping(const std::string & uri, const Handler & handler) {

	std::shared_ptr<RequestHandlerMapping> mapping = std::make_shared<RequestHandlerMapping>();
	mapping->uri = uri;
	mapping->expr = boost::regex("^" + uri + "(?<TRIX_URI_REST>.*)$");
	mapping->handler = handler;
	mapping->captureNames = this->getCaptureNamesFromRegex(uri);

	this->handlerMappings.push_back(mapping);
}

std::vector<std::string> Handler::getCaptureNamesFromRegex(const std::string & regex) {

	std::vector<std::string> names;

	boost::regex x("\\?<([^<>]+)>");

	boost::sregex_iterator res(regex.begin(), regex.end(), x);
	boost::sregex_iterator end;
	for (; res != end; ++res) {
		names.push_back((*res)[1]);
	}

	return names;
}

/** HandlerProxy *****************************/

Handler & HandlerProxy::getHandler() {
	return this->handler;
}

void HandlerProxy::addMapping(const std::string & uri, const func_t & func) {
	this->handler.addMapping(uri, func);
}

void HandlerProxy::addMapping(const std::string & uri, const unsigned short & methods, const func_t & func) {
	this->handler.addMapping(uri, methods, func);
}

void HandlerProxy::addHandlerMapping(const std::string & uri, const Handler & handler) {
	this->handler.addHandlerMapping(uri, handler);
}


} /* namespace trix */
