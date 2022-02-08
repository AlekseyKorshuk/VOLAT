#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <nlohmann/json.hpp>

#if _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else

#include "TcpClientSocket.hpp"

#endif

using json = nlohmann::json;

/**
 * Response structure
 */
struct response {
    int Result; // Result code
    json msg; // Message in json format
};


/**
 * Communicates with server using sockets
 */
class Client {
public:
    /**
     * Default constructor
     */
    Client();

    /**
     * Login action authenticate client and connect him to game.
     * @param name Player's name.
     * @param password Player's password used to verify the connection.
     * @param game Game's name (use it to connect to existing game).
     * @param num_turns Number of game turns to be played.
     * @param num_players Number of players in the game
     * @return Response
     */
    response
    login(std::string name, std::string password = "", std::string game = "", int num_turns = 45, int num_players = 3);

    /**
     * Logout and remove player record from server storage.
     * @return Response
     */
    response logout();

    /**
     * Returns the game map. Map represents static information about game.
     * @return Response
     */
    response map();

    /**
     * Returns current state of game. Game state represents dynamic information about game.
     * @return Response
     */
    response game_state();

    /**
     * Returns list of game actions that happened in previous turn. Represent changes between turns.
     * @return Response
     */
    response game_action();

    /**
     * Forces next turn of the game.
     * @return Response
     */
    response turn();

    /**
     * Do nothing. Just for testing and fun.
     * @param msg String message
     * @return Response
     */
    response chat(const std::string &msg);

    /**
     * Changes vehicle position.
     * @param vehicle_id Id of vehicle
     * @param x X coordinate of hex
     * @param y Y coordinate of hex
     * @param z Z coordinate of hex
     * @return Response
     */
    response move(int vehicle_id, int x, int y, int z);

    /**
     * Shoot to target position.
     * @param vehicle_id Id of vehicle
     * @param x X coordinate of hex
     * @param y Y coordinate of hex
     * @param z Z coordinate of hex
     * @return Response
     */
    response shoot(int vehicle_id, int x, int y, int z);

private:
    /**
     * Sends request to a server
     * @param action Action type
     * @param msg Message
     */
    void request(int action, std::string msg);

    /**
     * Receives response from server
     * @return Response
     */
    response answer();


    char inetAddr[100] = {"92.223.34.102"}; // Server address
    int port = 443; // Server port

    bool debugging = false; // Is debugging

#if _WIN32 // SOCKET for Windows
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
#else  // TcpClientSocket for Posix
    TcpClientSocket server;
#endif
};

