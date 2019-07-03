//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include <iostream>

#include "conn/ServerPlayer.h"
#include "conn/ServerClient.h"
#include "conn/ServerClients.h"
#include "world/ServerWorld.h"
#include "../util/net/Packet.h"
#include "../util/Timer.h"
#include "../util/net/NetHandler.h"
#include "../def/ServerDefs.h"

class Server {
public:
    explicit Server(unsigned short port);

    void update();
    void handlePlayerPacket(ServerClient& client, Packet& p);

    void cleanup();

    ~Server();
private:
    bool alive = true;

    ServerDefs defs;
    ServerWorld world;
    NetHandler handler;
    ServerClients clientList;

    double elapsedSeconds;
    double deltaTime;

    unsigned short port;
};


#endif //ZEUS_SERVER_H
