#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#pragma comment(lib, "WS2_32.lib")

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

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;


    char inetAddr[100] = {"92.223.34.102"};
    int port = 443;

    bool debugging = true;
};

