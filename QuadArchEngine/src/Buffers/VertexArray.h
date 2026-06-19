#pragma once
#include "QuadArch/Core.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

namespace QuadArch {
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_VertexArrayID;
	};
}