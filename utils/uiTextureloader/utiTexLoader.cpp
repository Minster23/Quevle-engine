#include <utils/uiTextureloader/utiTexLoader.hpp>
#include <stb_image.h>

unsigned int uiTexLoader::loadTex(const std::string &texLocation)
{
    if (texLocation.empty())
    {
        std::cerr << "Texture path is empty\n";
        return 0;
    }

    // Check if file exists first
    if (!std::filesystem::exists(texLocation))
    {
        std::cerr << "Texture file not found: " << texLocation << std::endl;
        return 0;
    }

    unsigned int texID = 0;
    glGenTextures(1, &texID);

    if (texID == 0)
    {
        std::cerr << "Failed to generate texture ID\n";
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texID);

    // Set some default texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(texLocation.c_str(), &width, &height, &nrChannels, 0);

    if (data && width > 0 && height > 0)
    {
        GLenum format = GL_RGB;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported texture format: " << nrChannels << " channels\n";
            stbi_image_free(data);
            glDeleteTextures(1, &texID);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        // Verify the texture was created successfully
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error after loading texture: " << error << std::endl;
            glDeleteTextures(1, &texID);
            return 0;
        }

        return texID;
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << texLocation << std::endl;
        if (data)
            stbi_image_free(data);
        glDeleteTextures(1, &texID);
        return 0;
    }
}
void uiTexLoader::unloadTex(unsigned int texID)
{
    if (texID != 0)
    {
        glDeleteTextures(1, &texID);
    }
}