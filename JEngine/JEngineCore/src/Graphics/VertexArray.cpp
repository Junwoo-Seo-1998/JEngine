#include"VertexArray.h"
#include <glad/glad.h>
VertexArray::~VertexArray()
{
	UnBind();
	glDeleteVertexArrays(1, &m_VertexArray);
}

std::shared_ptr<VertexArray> VertexArray::CreateVertexArray()
{
	return std::shared_ptr<VertexArray>(new VertexArray{});
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VertexArray);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

VertexArray::VertexArray()
	:m_VertexArray(0)
{
	glCreateVertexArrays(1, &m_VertexArray);
}
