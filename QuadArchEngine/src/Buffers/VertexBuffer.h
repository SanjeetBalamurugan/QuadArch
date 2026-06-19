#pragma once
#include "QuadArch/Core.h"

namespace QuadArch {
	class QuadAPI VertexBuffer
	{
	public:
		~VertexBuffer()
		{
			Destroy();
		}

		void CreateBuffer(const void* data, unsigned int size);
		void Bind() const;
		void Unbind() const;
		void Destroy();

	private:
		bool m_isDestroyed = false;
		unsigned int m_VertexBufferID;
	};
}

