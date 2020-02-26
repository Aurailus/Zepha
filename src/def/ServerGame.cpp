//
// Created by aurailus on 10/06/19.
//

#include "ServerGame.h"
#include "../server/conn/ClientList.h"

ServerGame::ServerGame(const std::string& subgame) :
    subgamePath("subgames/" + subgame + "/") {

    if (subgame.empty()) {
        std::cout << Log::err << "No subgame specified." << Log::endl;
        exit(1);
    }
    else if (!cf_file_exists(subgamePath.data())) {
        std::cout << Log::err << "Subgame '" << subgame << "' does not exist." << Log::endl;
        exit(1);
    }
}

void ServerGame::init(ServerWorld &world) {
    parser.init(*this, world, subgamePath);
}

void ServerGame::update(double delta, ClientList& clients) {
    parser.update(delta);
}
