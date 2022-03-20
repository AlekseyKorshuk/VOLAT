#include "Client.h"

#include <nlohmann/json.hpp>


using json = nlohmann::json;

Client::Client() {
#if _WIN32
    WSAStartup(MAKEWORD(2, 1), &WSAData);
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
    }

    addr.sin_addr.s_addr = inet_addr(inetAddr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (connect(server, (SOCKADDR *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout << "Server connection failed with error: " << WSAGetLastError() << std::endl;
    }
#else
    server = TcpClientSocket(inetAddr, port);
    server.openConnection();
#endif

}

response Client::login(std::string name, std::string password, std::string game, int num_turns, int num_players) {
    std::string msg = "{\"name\":\"" + name +
                 "\",\"password\":\"" + password +
                 "\",\"game\":\"" + game +
                 "\",\"num_turns\":" + std::to_string(num_turns) +
                 ",\"num_players\":" + std::to_string(num_players) + "}";
    request(1, msg);

    return answer();
}


response Client::logout() {
    request(2, "");
    return answer();
}


response Client::map() {
    request(3, "");
    return answer();
}

response Client::game_state() {
    request(4, "");
    return answer();
}

response Client::game_action() {
    request(5, "");
    return answer();
}

response Client::turn() {
    request(6, "");
    return answer();
}

response Client::chat(const std::string &msg) {
    request(100, "{\"message\":\"" + msg + "\"}");
    return answer();
}


response Client::move(int vehicle_id, int x, int y, int z) {
    std::string msg = "{\"vehicle_id\":" + std::to_string(vehicle_id) +
                 ",\"target\":{" +
                 "\"x\":" + std::to_string(x) +
                 ",\"y\":" + std::to_string(y) +
                 ",\"z\":" + std::to_string(z) + "}}";

    request(101, msg);
    return answer();
}

response Client::shoot(int vehicle_id, int x, int y, int z) {
    std::string msg = "{\"vehicle_id\":" + std::to_string(vehicle_id) +
                 ",\"target\":{" +
                 "\"x\":" + std::to_string(x) +
                 ",\"y\":" + std::to_string(y) +
                 ",\"z\":" + std::to_string(z) + "}}";

    request(102, msg);
    return answer();
}

void Client::request(int action, std::string msg) {
    char *buffer = new char[8 + msg.size()];

    for (int i = 0; i < 4; ++i) {
        buffer[i] = ((char *) &action)[i];
    }

    int size_msg = msg.size();
    for (int i = 0; i < 4; ++i) {
        buffer[i + 4] = ((char *) &size_msg)[i];
    }

    for (int i = 0; i < size_msg; i++) {
        buffer[i + 8] = msg[i];
    }

    if (debugging) std::cout << "Client: " << action << " " << msg << '\n';

#if _WIN32
    send(server, buffer, 8 + size_msg, 0);
#else
    server.sendData(buffer, 8 + size_msg);
#endif
}

response Client::answer() {
    char buffer[4] = {0};

#if _WIN32
    recv(server, buffer, 4, MSG_WAITALL);
#else
    server.receiveData(buffer, 4);
#endif
    int result =
            ((buffer[3] & 0xff) << 24) | ((buffer[2] & 0xff) << 16) | ((buffer[1] & 0xff) << 8) | (buffer[0] & 0xff);
#if _WIN32
    recv(server, buffer, 4, MSG_WAITALL);
#else
    server.receiveData(buffer, 4);
#endif
    int size_msg =
            ((buffer[3] & 0xff) << 24) | ((buffer[2] & 0xff) << 16) | ((buffer[1] & 0xff) << 8) | (buffer[0] & 0xff);
    if (size_msg == 0)
        return {result, json::parse("{}")};
    char *c_msg = new char[size_msg];
#if _WIN32
    if (size_msg != 0) recv(server, c_msg, size_msg, MSG_WAITALL);
#else
    if (size_msg != 0) server.receiveData(c_msg, size_msg);
#endif

    std::string msg;

    for (int i = 0; i < size_msg; i++) {
        msg += c_msg[i];
    }

    if (debugging) std::cout << "Server: " << result << " " << msg << "\n\n";

    return {result, json::parse(msg)};
}
