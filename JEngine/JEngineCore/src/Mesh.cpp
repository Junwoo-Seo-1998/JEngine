#include "Mesh.h"

MeshInstance::MeshInstance(Mesh& mesh)
{
	struct PosNormal
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		std::array<float, MAX_BONE_INFLUENCE>BoneIDs;
		std::array<float, MAX_BONE_INFLUENCE>Weights;
	};
	std::vector<PosNormal>refined;
	refined.reserve(mesh.vertices.size());

	for (auto& v:mesh.vertices)
	{
		PosNormal vertex;
		vertex.Position = v.Position;
		vertex.Normal = v.Normal;
		for (int i=0; i<vertex.BoneIDs.size(); ++i)
		{
			vertex.BoneIDs[i] = v.BoneIDs[i];
			vertex.Weights[i] = v.Weights[i];
		}
		refined.emplace_back(vertex);
	}

	m_Buffer = VertexBuffer::CreateVertexBuffer(sizeof(PosNormal) * mesh.vertices.size());
	m_Buffer->SetData(sizeof(PosNormal)*refined.size(), refined.data());

	m_Buffer->SetDataTypes({
			{0, DataType::Float3},//location=0, pos
			{1, DataType::Float3},//location=1, normal
			{2, DataType::Float4},//location=2, vec4
			{3, DataType::Float4},//location=3, vec4
		});

	m_IndexBuffer = IndexBuffer::CreateIndexBuffer(mesh.indices.size());
	m_IndexBuffer->SetData(mesh.indices.size(), mesh.indices.data());
}