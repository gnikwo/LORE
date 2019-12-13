#pragma once

#include <string>
#include <vector>

#include <gltf2/glTF2.hpp>

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

using std::string;
using std::vector;

namespace LORE {
    class Importer {
        public:
            Importer(string file);
            ~Importer();

            Scene* import();

        private:
            void importScene(gltf2::Asset asset, unsigned int i);
            void importCamera(gltf2::Asset asset, unsigned int i);
            void importNode(gltf2::Asset asset, unsigned int i);
            void importObject(gltf2::Asset asset, unsigned int i);
            void importMesh(gltf2::Asset asset, unsigned int i);
            void importMaterial(gltf2::Asset asset, unsigned int i);
            void importShader(gltf2::Asset asset, unsigned int i);
            void importTexture(gltf2::Asset asset, unsigned int i);

            void buildTree(gltf2::Asset asset, unsigned int i);
            void buildNodeTree(gltf2::Asset asset, Scene* scene, unsigned int i);

            vector<float> importData(gltf2::Asset asset, gltf2::Accessor accessor);

            string m__filePath;

            vector<Scene*> m__scenes;
            vector<Camera*> m__cameras;
            vector<Mesh*> m__meshes;
            vector<Node*> m__nodes;
            vector<Material*> m__materials;
            vector<Texture*> m__textures;
    };
}
