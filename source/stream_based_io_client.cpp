#include <boost/asio.hpp>
#include <iostream>
using namespace boost;
int main() {
    asio::ip::tcp::iostream stream("localhost", "8080");
    if (!stream) {
        std::cout << "Error occurred! Error code = " << stream.error().value()
                  << ". Message = " << stream.error().message() << std::endl;
        return -1;
    }
    stream << "Request.";
    stream.flush();
    std::cout << "Response: " << stream.rdbuf() << std::endl;
    //能连接，但是stream io没有分隔符，一直都在输出，只有客户端、服务端关闭才能跳出
    return 0;
}