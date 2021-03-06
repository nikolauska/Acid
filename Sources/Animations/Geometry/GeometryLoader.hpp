#pragma once

#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "VertexAnimated.hpp"
#include "VertexAnimatedData.hpp"

namespace acid
{
	class ACID_EXPORT GeometryLoader
	{
	private:
		std::shared_ptr<Metadata> m_meshData;

		std::vector<VertexSkinData> m_vertexWeights;

		std::vector<VertexAnimatedData *> m_positionsList;
		std::vector<Vector2> m_uvsList;
		std::vector<Vector3> m_normalsList;

		std::vector<IVertex *> m_vertices;
		std::vector<uint32_t> m_indices;
	public:
		GeometryLoader(const std::shared_ptr<Metadata> &libraryGeometries, const std::vector<VertexSkinData> &vertexWeights);

		~GeometryLoader();

		std::vector<IVertex *> GetVertices() const { return m_vertices; }

		std::vector<uint32_t> GetIndices() const { return m_indices; }
	private:
		void LoadVertices();

		void LoadUvs();

		void LoadNormals();

		void AssembleVertices();

		VertexAnimatedData *ProcessVertex(const int32_t &positionIndex, const int32_t &normalIndex, const int32_t &uvIndex);

		VertexAnimatedData *DealWithAlreadyProcessedVertex(VertexAnimatedData *previousVertex, const int32_t &newUvIndex, const int32_t &newNormalIndex);

		void RemoveUnusedVertices();
	};
}
