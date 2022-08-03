#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <cstdint>
#include <functional>
#include <thread>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

static constexpr uint16_t BUFFER_SIZE   = 4096;

struct TcpServer {
    class Client;
    typedef std::function<void(Client)> handler_function_t;
    enum class status : uint8_t {
        up = 0,
        err_socket_init = 1,
        err_socket_bind = 2,
        err_socket_listening = 3,
        close = 4
    };

private:
    uint16_t port;
    status _status = status::close;
    handler_function_t handler;

    std::thread handler_thread;
    std::list<std::thread> client_handler_threads;
    std::list<std::thread::id> client_handling_end;

#ifdef _WIN32 // Windows NT
    SOCKET serv_socket = INVALID_SOCKET;
    WSAData w_data;
#else // *nix
    int serv_socket;
#endif

    void handlingLoop();

public:
    TcpServer(uint16_t port, handler_function_t handler);
    ~TcpServer();

    //! Set client handler
    void setHandler(handler_function_t handler);

    uint16_t getPort() const;
    uint16_t setPort(uint16_t port);

    status getStatus() const {return _status;}

    status restart();
    status start();
    void stop();

    void joinLoop();
};

class TcpServer::Client {
#ifdef _WIN32 // Windows NT
    SOCKET socket;
    SOCKADDR_IN address;
    char buffer[buffer_size];
public:
    Client(SOCKET socket, SOCKADDR_IN address);
#else // *nix
    int socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
public:
    Client(int socket, struct sockaddr_in address);
#endif
public:
    Client(const Client& other);
    ~Client();
    uint32_t getHost() const;
    uint16_t getPort() const;

    int loadData();
    char* getData();

    bool sendData(const std::string& data) const;
};

#endif // TCPSERVER_H