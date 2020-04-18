//
// Created by aurailus on 05/03/19.
//

#pragma once

#include <unordered_set>

#include "ServerGenStream.h"
#include "../conn/ClientList.h"
#include "../../def/ServerGame.h"
#include "../../game/scene/world/World.h"
#include "../../world/ServerDimension.h"

class ServerWorld : public World {
public:
    const static int MB_GEN_H = 2;
    const static int MB_GEN_V = 2;

    explicit ServerWorld(unsigned int seed, ServerGame& defs, ClientList& clients);

    void init();
    void update(double delta) override;

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;

    ServerDimension dimension {};
private:
    void changedChunks(ServerClient& client);
    bool generateMapBlock(glm::ivec3 pos);

    void sendChunk(const glm::ivec3& pos, ServerClient& client);
    static void sendChunk(const std::shared_ptr<BlockChunk>& chunk, ServerClient& client);
    void sendMapBlock(const glm::ivec3& pos, ServerClient& client);

    static bool isInBounds(glm::ivec3 pos, std::pair<glm::ivec3, glm::ivec3>& bounds);

    std::unique_ptr<ServerGenStream> genStream = nullptr;

    unsigned int seed;
    ServerGame& defs;
    ClientList& clientList;

    unsigned int generatedChunks = 0;

    //Static vector of chunks to place around players
    std::vector<glm::ivec3> generateOrder;
};

