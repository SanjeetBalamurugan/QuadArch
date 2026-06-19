#include "pch.h"
#include "VertexArray.h"

QuadArch::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_VertexArrayID);
}

QuadArch::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void QuadArch::VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	this->Bind();
	vbo.Bind();

	auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(0, element.count, element.type,
			element.normalised, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void QuadArch::VertexArray::Bind() const
{
	glBindVertexArray(m_VertexArrayID);
}

void QuadArch::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
