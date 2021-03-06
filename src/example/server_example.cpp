
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "../server.hpp"
#include "../handler.hpp"
#include "../request.hpp"
#include "../response.hpp"

void test(const trix::Request & request, trix::Response & response) {

	response << "Uri params:<br />\r\n";
	for (auto const & entry : request.getUriParams()) {
		response << entry.first << " = " << entry.second << "<br />\r\n";
	}


	response << "<br />\r\nGet params:<br />\r\n";
	for (auto const & entry : request.getGetParams()) {
		response << entry.first << " = " << entry.second << "<br />\r\n";
	}

	response << "<br />\r\nPost params:<br />\r\n";
	for (auto const & entry : request.getPostParams()) {
		response << entry.first << " = " << entry.second << "<br />\r\n";
	}

}

void testAttachment(const trix::Request & request, trix::Response & response) {

	std::string filename = request.getUriParams().at("filename");

	std::string data = "super test of attachment downlaod using trix!";

	response.setContentType("text/plain");
	response.addHeader("Content-Disposition", "attachment; filename=" + filename);
	response.setContentLength(data.size());

	response.write(data.c_str(), data.size());

}

int main() {

	using namespace trix;

	Handler baseHandler;
	Handler subHandler;
	subHandler.addMapping("(?<month>\\d{2})/", boost::bind(test, _1, _2));
	baseHandler.addHandlerMapping("(?<year>\\d{4})/", subHandler);

	Server server("0.0.0.0", "8083");
	server.addHandlerMapping("/", baseHandler);
	server.addMapping("/bla/(?<test>huh)(?<test2>.*)/", boost::bind(test, _1, _2));
	server.addMapping("/static/(?<filename>[^/]+\\.txt)", boost::bind(testAttachment, _1, _2));
	server.run();

}
