#include <utils/uiTextureloader/utiTexLoader.hpp>
#include <stb_image.h>

unsigned int uiTexLoader::loadTex(const std::string& texLocation)
{
    if (texLocation.empty())
        return 0;

    // Generate texture ID
    unsigned int texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(texLocation.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        switch (nrChannels)
        {
        case 1:  format = GL_RED;  break;
        case 3:  format = GL_RGB;  break;
        case 4:  format = GL_RGBA; break;
        default:
            std::cerr << "Unsupported texture format: " << nrChannels << " channels\n";
            stbi_image_free(data);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << texLocation << std::endl;
        texID = 0;
    }

    stbi_image_free(data);
    return texID;
}