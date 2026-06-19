#include "pch.h"
#include "IndexBuffer.h"

namespace QuadArch {
	void IndexBuffer::CreateBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_IndexBufferID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Destroy()
	{
		if (!m_isDestroyed)
			glDeleteBuffers(1, &m_IndexBufferID);

		m_isDestroyed = true;
	}
}
