#include "pch.h"
#include "VertexBuffer.h"

namespace QuadArch {
	void VertexBuffer::CreateBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Destroy()
	{
		if (!m_isDestroyed)
			glDeleteBuffers(1, &m_VertexBufferID);

		m_isDestroyed = true;
	}
}
