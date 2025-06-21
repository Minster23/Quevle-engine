#include <core/model/model.hpp>
#include <utils/Debug.h>

using namespace QuavleEngine;

Model::Model(std::string const &path, bool gamma)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        DEBUG_PRINT("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
        return;
    }

    setModelData(scene, path);
}

void Model::setModelData(const aiScene *scene, const std::string &path)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        ObjectEntity::ObjectData objectData;

        // Name
        objectData.name = mesh->mName.C_Str();
        objectData.vertexCount = mesh->mNumVertices;

        // Init default OpenGL fields
        objectData.VAO = 0;
        objectData.VBO = 0;
        objectData.EBO = 0;
        objectData.TexCoords = mesh->HasTextureCoords(0) ? 1 : 0; // Assuming TexCoords is a boolean flag
        objectData.shaderProgram = 0;
        objectData.vertexShader = 0;
        objectData.fragmentShader = 0;
        objectData.diffuseTextureID = 0;
        objectData.specularTextureID = 0;

        // Vertices
        if (mesh->mNumVertices > 0) {
            size_t vertCount = mesh->mNumVertices;
            // Now supporting: position (3), normal (3), texcoord (2), tangent (3), bitangent (3), color (4)
            // We'll use 14 floats per vertex if all attributes are present
            bool hasNormals = mesh->HasNormals();
            bool hasTangents = mesh->HasTangentsAndBitangents();
            bool hasTexCoords = mesh->HasTextureCoords(0);
            bool hasColors = mesh->HasVertexColors(0);
            size_t floatsPerVertex = 3 + (hasNormals ? 3 : 0) + (hasTangents ? 6 : 0) + (hasTexCoords ? 2 : 0) + (hasColors ? 4 : 0);
            float* verts = new float[vertCount * floatsPerVertex];

            for (unsigned int v = 0; v < vertCount; ++v) {
                size_t offset = 0;
                // Position
                verts[v * floatsPerVertex + offset++] = mesh->mVertices[v].x;
                verts[v * floatsPerVertex + offset++] = mesh->mVertices[v].y;
                verts[v * floatsPerVertex + offset++] = mesh->mVertices[v].z;
                // Normal
                if (hasNormals) {
                    verts[v * floatsPerVertex + offset++] = mesh->mNormals[v].x;
                    verts[v * floatsPerVertex + offset++] = mesh->mNormals[v].y;
                    verts[v * floatsPerVertex + offset++] = mesh->mNormals[v].z;
                }
                // Tangent/Bitangent
                if (hasTangents) {
                    verts[v * floatsPerVertex + offset++] = mesh->mTangents[v].x;
                    verts[v * floatsPerVertex + offset++] = mesh->mTangents[v].y;
                    verts[v * floatsPerVertex + offset++] = mesh->mTangents[v].z;
                    verts[v * floatsPerVertex + offset++] = mesh->mBitangents[v].x;
                    verts[v * floatsPerVertex + offset++] = mesh->mBitangents[v].y;
                    verts[v * floatsPerVertex + offset++] = mesh->mBitangents[v].z;
                }
                // TexCoords
                if (hasTexCoords) {
                    verts[v * floatsPerVertex + offset++] = mesh->mTextureCoords[0][v].x;
                    verts[v * floatsPerVertex + offset++] = mesh->mTextureCoords[0][v].y;
                }
                // Vertex Color
                if (hasColors) {
                    verts[v * floatsPerVertex + offset++] = mesh->mColors[0][v].r;
                    verts[v * floatsPerVertex + offset++] = mesh->mColors[0][v].g;
                    verts[v * floatsPerVertex + offset++] = mesh->mColors[0][v].b;
                    verts[v * floatsPerVertex + offset++] = mesh->mColors[0][v].a;
                }
            }
            objectData.vertices = verts;
            objectData.floatsPerVertex = floatsPerVertex;
        } else {
            objectData.vertices = nullptr;
            objectData.floatsPerVertex = 0;
        }

        // Indices (EBO)
        if (mesh->mNumFaces > 0) {
            std::vector<unsigned int> indices;
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                    indices.push_back(face.mIndices[j]);
                }
            }
            objectData.indicesCount = indices.size();
            objectData.indices = new unsigned int[indices.size()];
            std::copy(indices.begin(), indices.end(), objectData.indices);
        } else {
            objectData.indices = nullptr;
            objectData.indicesCount = 0;
        }

        // Material
        if (mesh->mMaterialIndex >= 0 && scene->mMaterials) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            aiColor3D color(0.f, 0.f, 0.f);

            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
                objectData.material.diffuse = { color.r, color.g, color.b };
            }
            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
                objectData.material.specular = { color.r, color.g, color.b };
            }
            float shininess = 0.0f;
            if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess)) {
                objectData.material.shininess = shininess;
            }
        } else {
            objectData.material = {}; // Zero-initialize
        }

        // Store in static object list
        ObjectEntity::objects.push_back(objectData);
        DEBUG_PRINT("Processed mesh: " + objectData.name + " with " + std::to_string(mesh->mNumVertices) + " vertices and " + std::to_string(objectData.indicesCount) + " indices.");
    }
}
