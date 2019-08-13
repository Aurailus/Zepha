//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

DebugGui::DebugGui(glm::vec2 bufferSize, TextureAtlas& atlas) :
    displayMode(0) {

    auto fpsHistogramRef = atlas.getTextureRef("histogram");
    auto genericHistogramRef = atlas.getTextureRef("histogram_white");
    auto fontRef = atlas.getTextureRef("font");

    auto crosshairText = std::make_shared<GUIText>("crosshairText");
    crosshairText->create({2, 2}, {}, {0.1, 0.1, 0.1, 0.3}, {}, fontRef);
    add(crosshairText);

    auto dataText = std::make_shared<GUIText>("dataText");
    dataText->create({2, 2}, {}, {0.1, 0.1, 0.1, 0.3}, {}, fontRef);
    add(dataText);


    auto interpGraph = std::make_shared<GUILabelledGraph>("interpGraph");
    interpGraph->create({244, 64}, {}, "Interp", 120, 128, genericHistogramRef, fontRef);
    add(interpGraph);

    auto meshGraph = std::make_shared<GUILabelledGraph>("meshGraph");
    meshGraph->create({244, 64}, {}, "Mesh", 120, 128, genericHistogramRef, fontRef);
    add(meshGraph);

    auto genGraph = std::make_shared<GUILabelledGraph>("genGraph");
    genGraph->create({244, 64}, {}, "Gen", 120, 512, genericHistogramRef, fontRef);
    add(genGraph);

    auto packetGraph = std::make_shared<GUILabelledGraph>("packetGraph");
    packetGraph->create({244, 64}, {}, "Packets", 120, 512, genericHistogramRef, fontRef);
    add(packetGraph);


    auto fpsGraph = std::make_shared<GUILabelledGraph>("fpsGraph");
    fpsGraph->create({244, 64}, {}, "FPS", 120, 60, fpsHistogramRef, fontRef);
    add(fpsGraph);

    auto drawsGraph = std::make_shared<GUILabelledGraph>("drawsGraph");
    drawsGraph->create({244, 64}, {}, "Draw Calls", 120, 0, genericHistogramRef, fontRef);
    add(drawsGraph);

    auto gpuGraph = std::make_shared<GUILabelledGraph>("gpuGraph");
    gpuGraph->create({244, 64}, {}, "GPU", 120, 1, genericHistogramRef, fontRef);
    add(gpuGraph);

    positionElements(bufferSize);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth  = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    get<GUIText>("crosshairText")->setPos({bufferWidth / 2 + 22, bufferHeight / 2 - 7});
    get<GUIText>("dataText")->setPos(glm::vec3(10, 10, 0));

    get<GUILabelledGraph>("genGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 160});
    get<GUILabelledGraph>("packetGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 240});
    get<GUILabelledGraph>("meshGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 80});
    get<GUILabelledGraph>("interpGraph")->setPos({bufferWidth - 254, bufferHeight - 70});

    get<GUILabelledGraph>("fpsGraph")->setPos({10, bufferHeight - 70});
    get<GUILabelledGraph>("drawsGraph")->setPos({10, bufferHeight - 70 - 80});
    get<GUILabelledGraph>("gpuGraph")->setPos({bufferWidth - 254, 10});
}

void DebugGui::update(Player& player, LocalWorld& world, LocalDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {

    { //VRam Usage Graph (Top Right)
        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

        get<GUILabelledGraph>("gpuGraph")->pushValue(static_cast<int>(std::round(
                (videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0))
                / 100.0f);
    }

    { //Bottom Left Graphs
        get<GUILabelledGraph>("fpsGraph")->pushValue(static_cast<float>(fps));
        get<GUILabelledGraph>("drawsGraph")->pushValue(drawCalls);
    }

    { //Bottom Right Graphs
        get<GUILabelledGraph>("meshGraph")->pushValue(world.lastMeshUpdates);
        get<GUILabelledGraph>("interpGraph")->pushValue(world.lastGenUpdates);
        get<GUILabelledGraph>("genGraph")->pushValue(static_cast<float>(ssGen));
        get<GUILabelledGraph>("packetGraph")->pushValue(static_cast<float>(ssPack));
    }

    { //Top-left Data
        glm::vec3 footPos = TransPos::roundPos(player.getPos()) + glm::vec3(0, -2, 0);

        int blockID = world.getBlock(footPos);
        std::string on = defs.blocks().fromId(blockID).identifier;

        glm::vec3 playerPos = TransPos::roundPos(player.getPos());
        glm::vec3 chunkPos = TransPos::chunkFromVec(playerPos);

        //Block Coordinates offset from respective container
        glm::vec3 posOffsetFromChunk = TransPos::chunkLocalFromVec(playerPos);
        glm::vec3 posOffsetFromBlock = TransPos::mapBlockLocalFromVec(playerPos);
        glm::vec3 posOffsetFromRegion = TransPos::regionLocalFromVec(playerPos);

        glm::vec3 chunkCoordinate = TransPos::Dimension::chunkOffsetFromMapBlock(chunkPos);
        glm::vec3 mapBlockCoordinate = TransPos::Dimension::mapBlockOffsetFromRegion(chunkPos);
        glm::vec3 regionCoordinate = TransPos::Dimension::regionFromVec(chunkPos);

        auto thing = player.getPointedThing();
        std::string face =
            (thing.face == TOP)    ? "TOP" :
            (thing.face == BOTTOM) ? "BOTTOM" :
            (thing.face == LEFT)   ? "LEFT" :
            (thing.face == RIGHT)  ? "RIGHT" :
            (thing.face == FRONT)  ? "FRONT" :
            (thing.face == BACK)   ? "BACK" :
            "NONE";

        std::ostringstream str;

        using namespace Util;

        str << "Player: " << vecToString(playerPos);
        str << " (" << floatVecToString(player.getPos()) << ")" << std::endl << std::endl;

        str << "Chunk: " << vecToString(posOffsetFromChunk) << " [" << vecToString(chunkPos) << "]" << std::endl;
        str << "MapBlock: " << vecToString(posOffsetFromBlock) << std::endl;
        str << "Region: " << vecToString(posOffsetFromRegion) << std::endl << std::endl;

        str << "Ch: " << vecToString(chunkCoordinate) << ", ";
        str << "Mb: " << vecToString(mapBlockCoordinate) << ", ";
        str << "Rg: " << vecToString(regionCoordinate) << std::endl;

        str << "Vel: " << floatVecToString(player.getVel()) << std::endl;
        str << "Yaw: " << floatToString(player.getYaw()) << ", ";
        str << "Pitch: " << floatToString(player.getPitch()) << std::endl << std::endl;

        str << "Standing On: " << on << std::endl << std::endl;

        if (thing.blockDef != nullptr) str << "Pointing At: " << thing.blockDef->identifier << ", " << std::endl;
        str << "Pointed Position: " << vecToString(thing.pos) << std::endl;
        str << "Pointed Face: " << face << std::endl;

        get<GUIText>("dataText")->setText(str.str());
    }

    { //Crosshair Text
        auto thing = player.getPointedThing();

        std::ostringstream crossText;
        if (thing.blockDef != nullptr) crossText << thing.blockDef->identifier << std::endl;
        get<GUIText>("crosshairText")->setText(crossText.str());
    }
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    setVisible(displayMode == 0);
    get<GUILabelledGraph>("fpsGraph")->setVisible(displayMode != 1);
}