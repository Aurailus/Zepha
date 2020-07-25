//
// Created by aurailus on 11/07/19.
//

#include <gzip/decompress.hpp>

#include "ConnectScene.h"

#include "../ClientState.h"
#include "../../net/Packet.h"
#include "../../lua/LuaMod.h"
#include "../graph/Renderer.h"
#include "../../net/Address.h"
#include "../../net/PacketType.h"
#include "../../net/PacketView.h"
#include "../../def/gen/LocalBiomeAtlas.h"
#include "../../def/LocalDefinitionAtlas.h"
#include "../hud/components/basic/GuiText.h"
#include "../hud/components/basic/GuiRect.h"
#include "../../lua/parser/LocalLuaParser.h"
#include "../../net/server/asset/AssetType.h"

ConnectScene::ConnectScene(ClientState &state, Address addr) : Scene(state),
    connection(state.connection) {

    state.renderer.setClearColor(10, 10, 10);

    Font f(state.defs.textures, state.defs.textures["font"]);

    auto statusText = std::make_shared<GuiText>("statusText");
    statusText->create({2, 2}, {}, {}, {1, 1, 1, 1}, f);
    statusText->setText("Connecting...");
    statusText->setPos({32, 24});
    components.add(statusText);

    auto loadBar = std::make_shared<GuiRect>("loadBar");
    loadBar->create({1, 32}, {}, {0.17, 0.75, 0.93, 1});
    loadBar->setPos({0, state.renderer.window.getSize().y - 32});
    components.add(loadBar);

    connection.attemptConnect(std::move(addr));

    state.renderer.window.addResizeCallback("scene", [&](glm::ivec2 win) {
        components.get<GuiRect>("loadBar")->setPos({0, win.y - 32});
    });
}

void ConnectScene::update() {
    state.defs.textures.update();

    switch (connectState) {
        default:
            throw std::runtime_error("Invalid connection state.");

        case State::DONE:
        case State::FAILED_CONNECT:
            break;

        case State::CONNECTING:
            handleConnecting();
            break;

        case State::PROPERTIES: {
            components.get<GuiRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.1, 32});

            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                PacketView p(e.packet);

                if (p.type == PacketType::SERVER_INFO) {
                    auto statusText = components.get<GuiText>("statusText");
                    statusText->setText(statusText->getText() + "Received server properties.\n");

                    state.seed = p.d.read<unsigned int>();

                    connectState = State::IDENTIFIER_LIST;
                    Packet resp(PacketType::BLOCK_IDENTIFIER_LIST);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::IDENTIFIER_LIST: {
            components.get<GuiRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.2, 32});

            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                PacketView p(e.packet);

                if (p.type == PacketType::BLOCK_IDENTIFIER_LIST) {
                    auto statusText = components.get<GuiText>("statusText");
                    statusText->setText(statusText->getText() + "Received block index-identifier table.\n");

                    state.defs.defs->setIdentifiers(p.d.read<std::vector<std::string>>());

                    Packet resp(PacketType::BIOME_IDENTIFIER_LIST);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
                else if (p.type == PacketType::BIOME_IDENTIFIER_LIST) {
                    auto statusText = components.get<GuiText>("statusText");
                    statusText->setText(statusText->getText() + "Received biome index-identifier table.\nDownloading mods...\n");

                    state.defs.biomes->setIdentifiers(p.d.read<std::vector<std::string>>());

                    connectState = State::MODS;
                    Packet resp(PacketType::MODS);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::MODS: {
            components.get<GuiRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.4, 32});
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                PacketView p(e.packet);

                auto statusText = components.get<GuiText>("statusText");

                if (p.type == PacketType::MODS) {
                    auto luaMod = LuaMod::fromPacket(p);
                    statusText->setText(statusText->getText() + "Received mod " + luaMod.config.name + ".\n");
                    state.defs.lua->getHandler().addLuaMod(std::move(luaMod));
                }
                else if (p.type == PacketType::MOD_ORDER) {
                    state.defs.lua->getHandler().setModsOrder(p.d.read<std::vector<std::string>>());

                    statusText->setText(statusText->getText() + "Done downloading mods.\nReceived the mods order.\nDownloading media...\n");

                    connectState = State::MEDIA;
                    Packet resp(PacketType::MEDIA);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::MEDIA: {
            components.get<GuiRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.6, 32});

            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                PacketView p(e.packet);

                auto statusText = components.get<GuiText>("statusText");

                if (p.type == PacketType::MEDIA) {
                    AssetType t = static_cast<AssetType>(p.d.read<int>());
                    unsigned int count = 0;

                    while (t != AssetType::END) {
                        std::string assetName = p.d.read<std::string>();

                        if (t == AssetType::TEXTURE) {
                            int width = p.d.read<unsigned int>();
                            int height = p.d.read<unsigned int>();

                            std::string data = p.d.read<std::string>();
                            std::string uncompressed = gzip::decompress(data.data(), data.length());

                            state.defs.textures.addImage(
                                    reinterpret_cast<unsigned char *>(const_cast<char *>(uncompressed.data())),
                                    assetName, true, width, height);
                        }
                        else if (t == AssetType::MODEL) {
                            std::string format = p.d.read<std::string>();
                            std::string data = p.d.read<std::string>();

                            state.defs.models.models.insert({assetName, SerializedModel{assetName, data, format}});
                        }

                        t = static_cast<AssetType>(p.d.read<int>());
                        count++;
                    }

                    statusText->setText(statusText->getText() + "Received " + std::to_string(count) + "x media files.\n");
                }
                else if (p.type == PacketType::MEDIA_DONE) {
                    components.get<GuiRect>("loadBar")->setScale({state.renderer.window.getSize().x, 32});
                    statusText->setText(statusText->getText() + "Done downloading media.\nJoining world...\n");

                    connectState = State::DONE;
                    state.desiredState = "game";
                }
            }
            break;
        }
    }
}

void ConnectScene::handleConnecting() {
    Packet resp(PacketType::SERVER_INFO);
    auto statusText = components.get<GuiText>("statusText");

    switch (connection.getConnectionStatus()) {
        default:
            throw std::runtime_error("Uncaught connection error.");

        case ServerConnection::State::ENET_ERROR:
            throw std::runtime_error("Enet Initialization error.");
            break;

        case ServerConnection::State::FAILED_CONNECT:
            connectState = State::FAILED_CONNECT;
            statusText->setText(statusText->getText() + "\nFailed to connect :(\n");
            break;

        case ServerConnection::State::ATTEMPTING_CONNECT:
            connection.processConnecting();

            dotsTime += state.delta;
            if (dotsTime > 1) {
                dotsTime -= 1;
                statusText->setText(statusText->getText() + ".");
            }

            break;

        case ServerConnection::State::CONNECTED:
            connectState = State::PROPERTIES;
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
    renderer.enableTexture(&state.defs.textures.atlasTexture);

    components.draw(renderer);

    renderer.swapBuffers();
}

void ConnectScene::cleanup() {
    state.renderer.window.removeResizeCallback("scene");
}