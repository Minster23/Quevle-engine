#include <core/model/model.hpp>
#include <utils/msgWnd.hpp>
#include <core/interface/interface.hpp>
#include <core/renderer/renderer.hpp>
#include <core/interface/interface.hpp>
#include <core/model/UUID.hpp>


using namespace QuavleEngine;
Renderer renderForModel;
interface intfcForModel;
UUID uid;

Model::Model(std::string const &path, bool gamma)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        intfc.inputDebug("Warning", "ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
        return;
    }
    locationData = path.substr(0, path.find_last_of('/'));
    intfcForModel.inputDebug("Info", "Model Loaded from path:"+locationData);
    setModelData(scene, path);
}

void Model::setModelData(const aiScene *scene, const std::string &path)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        ObjectEntity::ObjectData objectData;

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
        objectData.position = glm::vec3(0.0f, 0.0f, 0.0f);
        objectData.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        objectData.scale = glm::vec3(1.0f, 1.0f, 1.0f);

        // Standard Assimp: Only positions, normals, texcoords
        size_t floatsPerVertex = 8; // 3 pos, 3 normal, 2 texcoord
        float *verts = new float[mesh->mNumVertices * floatsPerVertex];

        for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
        {
            // Position
            verts[v * floatsPerVertex + 0] = mesh->mVertices[v].x;
            verts[v * floatsPerVertex + 1] = mesh->mVertices[v].y;
            verts[v * floatsPerVertex + 2] = mesh->mVertices[v].z;
            // Normal
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
            // Texcoord (only first set)
            if (mesh->HasTextureCoords(0))
            {
                verts[v * floatsPerVertex + 6] = mesh->mTextureCoords[0][v].x;
                verts[v * floatsPerVertex + 7] = mesh->mTextureCoords[0][v].y; // Use V as-is, no flip
            }
            else
            {
                verts[v * floatsPerVertex + 6] = 0.0f;
                verts[v * floatsPerVertex + 7] = 0.0f;
            }
        }

        objectData.vertices = verts;
        objectData.floatsPerVertex = floatsPerVertex;

        // Indeks
        if (mesh->mNumFaces > 0)
        {
            std::vector<unsigned int> indices;
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                aiFace face = mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; ++j)
                {
                    indices.push_back(face.mIndices[j]);
                }
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

        // Material
        if (mesh->mMaterialIndex >= 0 && scene->mMaterials)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            aiColor3D color(0.f, 0.f, 0.f);


            // * Default
            objectData.material.diffuse = {1.0f, 1.0f, 1.0f};
            objectData.material.specular = {1.0f, 1.0f, 1.0f};
            objectData.material.shininess = 1.0f;

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData +"/"+ std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::DIFFUSE);
                    intfcForModel.inputDebug("Info", "loading DIFFUSE map");
                }
            }
            if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData +"/"+ std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::SPECULAR);
                    intfcForModel.inputDebug("Info", "loading SPECULAR map");
                }
            }
            if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_METALNESS, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData +"/"+ std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::METALLIC);
                    intfcForModel.inputDebug("Info", "loading METALLIC map");
                }
            }
            if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData +"/"+ std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::NORMAL);
                    intfcForModel.inputDebug("Info", "loading NORMAL map");
                }
            }
            if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_HEIGHT, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::HEIGHT);
                    intfcForModel.inputDebug("Info", "loading HEIGHT map");
                }
            }

            if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_AMBIENT, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::AMBIENT);
                    intfcForModel.inputDebug("Info", "loading AMBIENT map");
                }
            }

            if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_EMISSIVE, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::EMISSIVE);
                    intfcForModel.inputDebug("Info", "loading EMISSIVE map");
                }
            }

            if (material->GetTextureCount(aiTextureType_OPACITY) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_OPACITY, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::OPACITY);
                    intfcForModel.inputDebug("Info", "loading OPACITY map");
                }
            }

            if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::DISPLACEMENT);
                    intfcForModel.inputDebug("Info", "loading DISPLACEMENT map");
                }
            }

            if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
            {
                aiString texPath;
                if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texPath) == AI_SUCCESS)
                {
                    std::string textureFile = locationData + "/" + std::string(texPath.C_Str());
                    std::cout << "Texture path: " << textureFile << std::endl;
                    renderForModel.loadTexture(textureFile, ObjectEntity::objects.size() - 1, Renderer::TextureType::ROUGHNESS);
                    intfcForModel.inputDebug("Info", "loading ROUGHNESS map");
                }
            }
        }

        else
        {
            objectData.material = {};
            intfcForModel.inputDebug("Warning","Item without material"); 
        }
    }
}
