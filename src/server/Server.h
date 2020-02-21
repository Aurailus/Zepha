//
// Created by aurailus on 09/01/19.
//

#pragma once

#include "conn/ClientList.h"
#include "world/ServerWorld.h"
#include "config/ServerConfig.h"
#include "../def/ServerGame.h"
#include "../util/net/NetHandler.h"

class ServerClient;
class Packet;

class Server {
public:
    explicit Server(const std::string& path, unsigned short port, const std::string& subgame);

    void update();
    void handlePlayerPacket(ServerClient& client, Packet& p);

    void cleanup();

    ~Server();
private:
    bool alive = true;

    ServerGame defs;
    ServerWorld world;
    NetHandler handler;
    ClientList clientList;
    ServerConfig config;

    std::unordered_map<unsigned int, bool> playersUpdated {};

    double elapsedSeconds = 0;
    double deltaTime = 0;

    unsigned short port = 0;
};

