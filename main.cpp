#include <iostream>

#include "server.hpp"
#include "messages.hpp"
#include "cute_little.hpp"

static constexpr uint16_t PORT = 6669;

static constexpr auto CMD_PUT = "PUT";
static constexpr auto CMD_GET = "GET";

static constexpr auto PARAM_CMD         = "cmd";
static constexpr auto PARAM_COLLECTION  = "cll";
static constexpr auto PARAM_OBJECT      = "obj";

cute_little db;

cute_little::object putObject(const cute_little::object& requestObject) {
    std::string collectionName          = requestObject[PARAM_COLLECTION];
    const cute_little::object& object   = requestObject[PARAM_OBJECT];

    auto &collection = db.get_collection(collectionName);
    return collection.add_object(object);
}

cute_little::object getObject(const cute_little::object& requestObject) {
    std::string collectionName = requestObject[PARAM_COLLECTION];
    size_t index = requestObject[PARAM_OBJECT];

    auto &collection = db.get_collection(collectionName);
    return collection.get_object(index);
}

cute_little::object executeRequest(const cute_little::object& requestObject) {
    std::string command = requestObject[PARAM_CMD];
    if (command == CMD_PUT)         { return putObject(requestObject); }
    else if (command == CMD_GET)    { return getObject(requestObject); }
}

void handler(TcpServer::Client client) {
    int size = 0;
    while (size == 0) size = client.loadData();
    std::string data = client.getData();

    std::cout << "REQ: " << data << std::endl;

    cute_little::object requestObject  = cute_little::object::parse(data);
    cute_little::object responseObject = executeRequest(requestObject);

    std::cout << "RESP: " << to_string(responseObject) <<  std::endl;
    //db.DBG_print();
    client.sendData(to_string(responseObject));
}

int main() {
    TcpServer server(PORT, handler);

    if(server.start() == TcpServer::status::up) {
        std::cout << INFO_SERVER_IS_UP << std::endl;
        server.joinLoop();
    } else {
        std::cout << ERR_COULD_NOT_START << int(server.getStatus()) << std::endl;
        return -1;
    }
}