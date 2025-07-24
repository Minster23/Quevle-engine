#include <core/model/model.hpp>
#include <utils/msgWnd.hpp>
#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <core/model/UUID.hpp>
#include <future>
#include <mutex>
#include <vector>
#include <functional>
#include <iostream>
#include <core/scene/scene.hpp>

using namespace QuavleEngine;
using namespace SCENEMANAGER;

Renderer renderForModel;
interface intfcForModel;
UUID uid;
scene sceneForModel;

// Queue to defer OpenGL texture calls
std::mutex textureQueueMutex;
std::vector<std::function<void()>> textureLoadQueue;

// Called from main thread (e.g., in render loop or init)
void Model::flushTextureLoadQueue()
{
    std::lock_guard<std::mutex> lock(textureQueueMutex);
    for (auto &fn : textureLoadQueue)
        fn();
    textureLoadQueue.clear();
}

void Model::model(const std::string &path, bool gamma)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_OptimizeMeshes |
        aiProcess_RemoveRedundantMaterials);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        intfcForModel.inputDebug("Warning", "ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
        return;
    }

    locationData = path.substr(0, path.find_last_of('/'));
    intfcForModel.inputDebug("Info", "Model Loaded from path:" + locationData);
    sceneForModel.scenes[sceneForModel.getSceneIndexByUUID(SCENEMANAGER::scene::selectedSceneUUID)].objectLoctions.push_back(path);
    setModelData(scene, path);
}

void Model::setModelData(const aiScene *scene, const std::string &path)
{
    std::vector<std::future<void>> futures;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        ObjectEntity::ObjectData objectData;

        objectData.locationMode = path;
        objectData.UUID = uid.generate_uuid();
        objectData.name = mesh->mName.C_Str();
        objectData.vertexCount = mesh->mNumVertices;

        objectData.VAO = 0;
        objectData.VBO = 0;
        objectData.EBO = 0;
        objectData.TexCoords = mesh->HasTextureCoords(0) ? 1 : 0;
        objectData.shaderProgram = 0;
        objectData.vertexShader = 0;
        objectData.fragmentShader = 0;
        objectData.diffuseTextureID = 0;
        objectData.specularTextureID = 0;
        objectData.position = glm::vec3(0.0f);
        objectData.rotation = glm::vec3(0.0f);
        objectData.scale = glm::vec3(1.0f);

        size_t floatsPerVertex = 8;
        float *verts = new float[mesh->mNumVertices * floatsPerVertex];

        for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
        {
            verts[v * floatsPerVertex + 0] = mesh->mVertices[v].x;
            verts[v * floatsPerVertex + 1] = mesh->mVertices[v].y;
            verts[v * floatsPerVertex + 2] = mesh->mVertices[v].z;

            if (mesh->HasNormals())
            {
                verts[v * floatsPerVertex + 3] = mesh->mNormals[v].x;
                verts[v * floatsPerVertex + 4] = mesh->mNormals[v].y;
                verts[v * floatsPerVertex + 5] = mesh->mNormals[v].z;
            }
            else
            {
                verts[v * floatsPerVertex + 3] = 0.0f;
                verts[v * floatsPerVertex + 4] = 0.0f;
                verts[v * floatsPerVertex + 5] = 0.0f;
            }

            if (mesh->HasTextureCoords(0))
            {
                verts[v * floatsPerVertex + 6] = mesh->mTextureCoords[0][v].x;
                verts[v * floatsPerVertex + 7] = mesh->mTextureCoords[0][v].y;
            }
            else
            {
                verts[v * floatsPerVertex + 6] = 0.0f;
                verts[v * floatsPerVertex + 7] = 0.0f;
            }
        }

        objectData.vertices = verts;
        objectData.floatsPerVertex = floatsPerVertex;

        if (mesh->mNumFaces > 0)
        {
            std::vector<unsigned int> indices;
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                aiFace face = mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }

            objectData.indicesCount = indices.size();
            objectData.indices = new unsigned int[indices.size()];
            std::copy(indices.begin(), indices.end(), objectData.indices);
        }
        else
        {
            objectData.indices = nullptr;
            objectData.indicesCount = 0;
        }

        ObjectEntity::objects.push_back(objectData);

        if (mesh->mMaterialIndex >= 0 && scene->mMaterials)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            size_t objectIndex = ObjectEntity::objects.size() - 1;

            // Default material
            ObjectEntity::objects[objectIndex].material.diffuse = {1.0f, 1.0f, 1.0f};
            ObjectEntity::objects[objectIndex].material.specular = {1.0f, 1.0f, 1.0f};
            ObjectEntity::objects[objectIndex].material.shininess = 1.0f;

            futures.push_back(std::async(std::launch::async, [=]() {
                std::vector<std::pair<aiTextureType, Renderer::TextureType>> types = {
                    {aiTextureType_DIFFUSE, Renderer::TextureType::DIFFUSE},
                    {aiTextureType_SPECULAR, Renderer::TextureType::SPECULAR},
                    {aiTextureType_METALNESS, Renderer::TextureType::METALLIC},
                    {aiTextureType_NORMALS, Renderer::TextureType::NORMAL},
                    {aiTextureType_HEIGHT, Renderer::TextureType::HEIGHT},
                    {aiTextureType_AMBIENT, Renderer::TextureType::AMBIENT},
                    {aiTextureType_EMISSIVE, Renderer::TextureType::EMISSIVE},
                    {aiTextureType_OPACITY, Renderer::TextureType::OPACITY},
                    {aiTextureType_DISPLACEMENT, Renderer::TextureType::DISPLACEMENT},
                    {aiTextureType_DIFFUSE_ROUGHNESS, Renderer::TextureType::ROUGHNESS},
                };

                for (auto &[assimpType, renderType] : types)
                {
                    if (material->GetTextureCount(assimpType) > 0)
                    {
                        aiString texPath;
                        if (material->GetTexture(assimpType, 0, &texPath) == AI_SUCCESS)
                        {
                            std::string textureFile = locationData + "/" + std::string(texPath.C_Str());

                            {
                                std::lock_guard<std::mutex> lock(textureQueueMutex);
                                textureLoadQueue.push_back([textureFile, objectIndex, renderType]() {
                                    renderForModel.loadTexture(textureFile, objectIndex, renderType);
                                    std::cout << "Loaded texture on main thread: " << textureFile << std::endl;
                                });
                            }

                            intfcForModel.inputDebug("Info", "Queued texture: " + textureFile);
                        }
                    }
                }
            }));
        }
        else
        {
            objectData.material = {};
            intfcForModel.inputDebug("Warning", "Item without material");
        }
    }

    for (auto &f : futures) f.wait();

    for (size_t i = 0; i < ObjectEntity::objects.size(); ++i)
    {
        renderForModel.shaderLoader(i, Renderer::RenderType::OBJECT);
        renderForModel.shaderLink(i, Renderer::RenderType::OBJECT);
    }

    intfcForModel.inputDebug("Info", "Model loading complete");
}
