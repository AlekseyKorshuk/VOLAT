#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstring>
#if _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32.lib")
#else
    #include "TcpClientSocket.hpp"
#endif
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

struct response {
    int Result;
    json msg;
};

class Client {
public:

    Client();

    response login(string name, string password="", string game="", int num_turns=45, int num_players=3);

//    response login(string name, string password, string game);

    response logout();

    response map();

    response game_state();

    response game_action();

    response turn();

    response chat(string msg);

    response move(int vehicle_id, int x, int y, int z);

    response shoot(int vehicle_id, int x, int y, int z);

private:

    void request(int action, string msg);

    response answer();




    char inetAddr[100] = {"92.223.34.102"};
    int port = 443;

    bool debugging = false;

    #if _WIN32
        WSADATA WSAData;
        SOCKET server;
        SOCKADDR_IN addr;
    #else
        TcpClientSocket server;
    #endif
};

