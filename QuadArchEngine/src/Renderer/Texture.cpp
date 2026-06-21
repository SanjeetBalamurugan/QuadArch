#include "pch.h"
#include "Texture.h"

namespace QuadArch
{
    Texture::Texture(const std::string& file_path)
    {
        //stbi_set_flip_vertically_on_load(true);
        m_data.data = stbi_load(file_path.c_str(), &m_data.width, &m_data.height, &m_data.nrChannels, 4);

        if (!m_data.data)
        {
            std::cout << "Failed to load texture file: " << file_path << std::endl;
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_data.width, m_data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(m_data.data);
        m_data.data = nullptr;

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture()
    {
        if (m_TextureID != 0)
        {
            glDeleteTextures(1, &m_TextureID);
        }
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}