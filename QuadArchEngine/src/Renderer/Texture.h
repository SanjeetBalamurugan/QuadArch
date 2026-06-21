#pragma once
#include "QuadArch/Core.h"

#include <string>

namespace QuadArch
{
	struct TextureData
	{
		int width;
		int height;
		int nrChannels;

		unsigned char* data;
	};

	class QuadAPI Texture
	{
	public:
		Texture(const std::string& file_path);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void UnBind() const;
	private:
		TextureData m_data;
		unsigned int m_TextureID;
	};
}
