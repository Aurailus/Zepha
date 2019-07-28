//
// Created by aurailus on 11/07/19.
//

#include "ConnectScene.h"

ConnectScene::ConnectScene(ClientState &state, Address addr) : Scene(state),
    connection(state.connection) {

    state.renderer.setClearColor(10, 10, 10);

    auto statusText = std::make_shared<GUIText>("statusText");
    statusText->create({2, 2}, {}, {}, {}, state.defs.textures().getTextureRef("font"));
    statusText->setText("Connecting...");
    statusText->setPos({32, 24});
    components.add(statusText);

    connection.attemptConnect(std::move(addr));
}

void ConnectScene::update() {
    switch (connectState) {
        default:
            std::cout << Log::err << "Invalid connectState" << Log::endl;
            exit(1);

        case State::FAILED_CONNECT:
            break;

        case State::CONNECTING:
            handleConnecting();
            break;

        case State::IDENTIFIER_LIST: {
            ENetEvent e;
            while (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);

                auto statusText = components.get<GUIText>("statusText");
                statusText->setText(statusText->getText() + "Recieved block index-identifier table.\n");

                std::vector<std::string> indexIdentifierTable {};
                indexIdentifierTable.reserve(static_cast<unsigned long>(Serializer::decodeInt(&p.data[0])));

                unsigned int ind = 4;
                while (true) {
                    auto len = Serializer::decodeInt(&p.data[ind]);
                    indexIdentifierTable.emplace_back(&p.data[ind + 4], &p.data[ind + 4 + len]);
                    ind += 4 + len;
                    if (ind >= p.data.length() - 4) break;
                }

                statusText->setText(statusText->getText() + "Joining World...");
                state.defs.blocks().setIdentifiers(indexIdentifierTable);
                state.desiredState = "game";
                return;
            }
        }
    }
}

void ConnectScene::handleConnecting() {
    Packet resp(PacketType::IDENTIFIER_LIST);
    auto statusText = components.get<GUIText>("statusText");

    switch (connection.getConnectionStatus()) {
        default:
            std::cout << Log::err << "Undefined connection error. Exiting." << Log::endl;

        case ServerConnection::State::ENET_ERROR:
            exit(1);
            break;

        case ServerConnection::State::FAILED_CONNECT:
            connectState = State::FAILED_CONNECT;
            statusText->setText(statusText->getText() + "\nFailed to connect :(\n");
            break;

        case ServerConnection::State::ATTEMPTING_CONNECT:
            connection.processConnecting();

            dotsTime += state.deltaTime;
            if (dotsTime > 1) {
                dotsTime -= 1;
                statusText->setText(statusText->getText() + ".");
            }

            break;

        case ServerConnection::State::CONNECTED:
            connectState = State::IDENTIFIER_LIST;
            statusText->setText(statusText->getText() + " Connected!~\n");

            resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);

            break;
    }
}

void ConnectScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&state.defs.textures().getAtlasTexture());

    components.draw(renderer);

    renderer.swapBuffers();
}