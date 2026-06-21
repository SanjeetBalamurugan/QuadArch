#include "pch.h"
#include "Texture.h"

QuadArch::Texture::Texture(const std::string& file_path)
	: m_TextureID(0), m_data{ 0, 0, 0, nullptr }
{
	stbi_set_flip_vertically_on_load(true);
	m_data.data = stbi_load(file_path.c_str(), &m_data.width, &m_data.height, &m_data.nrChannels, 0);

	if (m_data.data)
	{
		GLenum internalFormat = 0;
		GLenum dataFormat = 0;

		if (m_data.nrChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_data.nrChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_data.width, m_data.height, 0, dataFormat, GL_UNSIGNED_BYTE, m_data.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(m_data.data);
		m_data.data = nullptr;
	}
	else
	{
		std::cout << "Failed to load texture: " << file_path << std::endl;
	}
}

QuadArch::Texture::Texture(unsigned int rendererID)
	: m_TextureID(rendererID), m_data{ 0, 0, 0, nullptr }
{
}

QuadArch::Texture::~Texture()
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
	}
}

void QuadArch::Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void QuadArch::Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<QuadArch::Texture> QuadArch::Texture::LoadCubemap(const std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return std::make_shared<Texture>(textureID);
}

std::shared_ptr<QuadArch::Texture> QuadArch::Texture::LoadCrossCubemap(const std::string& file_path)
{
	stbi_set_flip_vertically_on_load(false);

	int masterWidth, masterHeight, nrChannels;
	unsigned char* masterData = stbi_load(file_path.c_str(), &masterWidth, &masterHeight, &nrChannels, 0);

	if (!masterData)
	{
		std::cout << "Failed to load cross cubemap file: " << file_path << std::endl;
		return nullptr;
	}

	int faceSize = masterWidth / 4;
	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	struct FaceMapping {
		GLenum target;
		int gridX;
		int gridY;
	};

	FaceMapping mappings[6] = {
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, 2, 1 }, // Right
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 1 }, // Left
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 1, 0 }, // Top
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 1, 2 }, // Bottom
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 1, 1 }, // Front
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3, 1 }  // Back
	};

	unsigned char* subFaceBuffer = new unsigned char[faceSize * faceSize * nrChannels];

	for (unsigned int i = 0; i < 6; i++)
	{
		int startX = mappings[i].gridX * faceSize;
		int startY = mappings[i].gridY * faceSize;

		for (int row = 0; row < faceSize; row++)
		{
			int currentY = startY + row;
			unsigned char* srcRowPtr = masterData + ((currentY * masterWidth) + startX) * nrChannels;
			unsigned char* dstRowPtr = subFaceBuffer + (row * faceSize) * nrChannels;
			std::memcpy(dstRowPtr, srcRowPtr, faceSize * nrChannels);
		}

		glTexImage2D(mappings[i].target, 0, format, faceSize, faceSize, 0, format, GL_UNSIGNED_BYTE, subFaceBuffer);
	}

	delete[] subFaceBuffer;
	stbi_image_free(masterData);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return std::make_shared<Texture>(textureID);
}
