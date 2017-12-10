#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <json.hpp>

class game_state : public boost::noncopyable {

private:
    int counter = 0;

public:
    void process_http_request(boost::beast::http::request<boost::beast::http::dynamic_body> &req,
                              boost::beast::http::response<boost::beast::http::dynamic_body> &res) {

        nlohmann::json request_json = nlohmann::json::parse(
            boost::beast::buffers_to_string(
                    // boost::beast::buffers(
                            req.body().data()
                    //)
            )
        );
        nlohmann::json response_json;

        response_json = request_json;
        response_json["dupa"] = "dupa 123";

        res.set(boost::beast::http::field::content_type, "application/json");
        boost::beast::ostream(res.body()) << response_json << std::endl;
    }

};

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    // rvalue - jednorazowe wykorzystanie - bez kopiowania
    http_connection(boost::asio::ip::tcp::socket && socket, game_state & gs):
    // std::move wskazanie że obj będzie nie potrzebny - (przenoszenie bez żadne kopii - przyspieszenie)
            socket(std::move(socket)), gs(gs)
    {

    }

private:
    boost::asio::ip::tcp::socket socket;
    boost::beast::flat_buffer buffer { 8192 };
    boost::beast::http::request<boost::beast::http::dynamic_body> request;
    boost::beast::http::response<boost::beast::http::dynamic_body> response;

    game_state & gs;


public:
    void start() {
        read_request();
    }

private:
    void read_request() {
        auto self = shared_from_this();

        boost::beast::http::async_read(
          socket,
          buffer,
          request,
          [self] (boost::beast::error_code ec, std::size_t bytes_transferred) {
              boost::ignore_unused(bytes_transferred);
              if(!ec)
                  self->process_request();
          }
        );
    }

    void process_request() {
        gs.process_http_request(request, response);
        write_response();
    }

    void write_response() {
        auto self = shared_from_this();


        response.keep_alive(false);
        response.version(request.version());

        response.set(boost::beast::http::field::content_length, response.body().size());

        boost::beast::http::async_write(
          socket,
          response,
          [self] (boost::beast::error_code ec, std::size_t) {
              self->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
          }
        );
    }
};


void accept_connection(boost::asio::ip::tcp::acceptor & acceptor, boost::asio::ip::tcp::socket & socket, game_state & gs) {
    acceptor.async_accept(socket, [&] (boost::system::error_code ec) {

        if(!ec)
            std::make_shared<http_connection>(std::move(socket), gs)->start();

        accept_connection(acceptor, socket, gs);
    });
}
int main() {

    game_state gs;

    boost::asio::io_service ioc;

    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::address_v4::any(), 8080};
    boost::asio::ip::tcp::acceptor acceptor(ioc, endpoint);
    boost::asio::ip::tcp::socket socket{ioc};

    accept_connection(acceptor, socket, gs);

    ioc.run();

    return 0;
}