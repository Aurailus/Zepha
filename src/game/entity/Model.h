//
// Created by aurailus on 22/08/19.
//

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "ModelBone.h"
#include "ModelAnimation.h"
#include "../graph/meshtypes/EntityMesh.h"
#include "../../def/texture/TextureAtlas.h"
#include "../../util/Mat4Conv.h"
#include "../../def/model/SerializedModel.h"

class Model {
public:
    Model() = default;

    void fromMesh(std::unique_ptr<EntityMesh> mesh);
    int  fromFile(const std::string &path, const std::vector<std::shared_ptr<AtlasRef>> &texture);
    int  fromSerialized(const SerializedModel &model, const std::vector<std::shared_ptr<AtlasRef>> &texture);

    void getTransformsByFrame(double frame, std::tuple<unsigned int, unsigned int> bounds, std::vector<glm::mat4>& transforms);
//    void getTransformsByTime(double time, std::tuple<uint> bounds, std::vector<glm::mat4>& transforms);

    const ModelAnimation& getAnimation();

    std::vector<std::unique_ptr<EntityMesh>> meshes;
private:
    void loadModelMeshes(aiNode *node, const aiScene *scene);
    void loadMeshAndBone(aiMesh *mesh, std::unique_ptr<EntityMesh> &target);
    void loadAnimations(const aiScene *scene);

    void calcBoneHeirarchy(aiNode *node, const aiScene *scene, int parentBoneIndex);
    void calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform, std::tuple<unsigned int, unsigned int> bounds, std::vector<glm::mat4>& transforms);

    void calcInterpolatedPosition(glm::vec3& position, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<unsigned int, unsigned int> bounds);
    void calcInterpolatedRotation(aiQuaternion& rotation, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<unsigned int, unsigned int> bounds);
    void calcInterpolatedScale(glm::vec3& scale, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<unsigned int, unsigned int> bounds);

    static inline unsigned int findPositionIndex(double animTime, AnimChannel &channel) {
        for (unsigned int i = 1; i < channel.positionKeys.size(); i++) {
            if (channel.positionKeys[i].first > animTime) return i - 1;
        }
        assert(false);
    }

    static inline unsigned int findRotationIndex(double animTime, AnimChannel &channel) {
        for (unsigned int i = 1; i < channel.rotationKeys.size(); i++) {
            if (channel.rotationKeys[i].first > animTime) return i - 1;
        }
        assert(false);
    }

    static inline unsigned int findScaleIndex(double animTime, AnimChannel &channel) {
        for (unsigned int i = 1; i < channel.scaleKeys.size(); i++) {
            if (channel.scaleKeys[i].first > animTime) return i - 1;
        }
        assert(false);
    }

    ModelAnimation animation {};
    std::vector<ModelBone*> rootBones {};
    std::vector<ModelBone> bones {};
    std::vector<std::shared_ptr<AtlasRef>> textures {};

    glm::mat4 globalInverseTransform {};
};

