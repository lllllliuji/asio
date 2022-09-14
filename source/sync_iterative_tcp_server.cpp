#include <atomic>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
using namespace boost;
class Service {
   public:
    Service() {}
    void HandleClient(asio::ip::tcp::socket& sock) {
        try {
            asio::streambuf request;
            asio::read_until(sock, request, '\n');
            // Emulate request processing.
            int i = 0;
            while (i != 1000000) i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            // Sending response.
            std::string response = "Response\n";
            asio::write(sock, asio::buffer(response));
        } catch (system::system_error& e) {
            std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
        }
    }
};
class Acceptor {
   public:
    Acceptor(asio::io_service& ios, unsigned short port_num) : m_ios(ios), m_acceptor(m_ios) {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port_num);
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor.bind(endpoint);
        m_acceptor.listen();
    }
    void Accept() {
        asio::ip::tcp::socket sock(m_ios);
        m_acceptor.accept(sock);
        std::cout << "remote: " << sock.remote_endpoint().address() << std::endl;
        Service svc;
        svc.HandleClient(sock);
    }

   private:
    asio::io_service& m_ios;
    asio::ip::tcp::acceptor m_acceptor;
};
class Server {
   public:
    Server() : m_stop(false) {}
    void Start(unsigned short port_num) {
        m_thread.reset(new std::thread([this, port_num]() { Run(port_num); }));
    }
    void Stop() {
        m_stop.store(true);
        m_thread->join();
    }

   private:
    void Run(unsigned short port_num) {
        Acceptor acc(m_ios, port_num);
        std::cout << "sync_iterative_server started" << std::endl;
        while (!m_stop.load()) {
            acc.Accept();
        }
    }
    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_stop;
    asio::io_service m_ios;
};
int main() {
    unsigned short port_num = 12345;
    try {
        Server srv;
        srv.Start(port_num);
        std::this_thread::sleep_for(std::chrono::seconds(600));
        srv.Stop();
    } catch (system::system_error& e) {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
    }
    return 0;
}