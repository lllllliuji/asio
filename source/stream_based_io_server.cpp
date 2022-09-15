#include <boost/asio.hpp>
#include <iostream>
using namespace boost;
int main() {
    asio::io_service io_service;
    asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

    asio::ip::tcp::iostream stream;
    acceptor.accept(*stream.rdbuf());
    std::cout << "Request: " << stream.rdbuf() << std::endl;
    stream << "Response.";
    stream.flush();
    return 0;
}