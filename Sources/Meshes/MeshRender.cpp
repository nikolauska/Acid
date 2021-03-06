#include "MeshRender.hpp"

#include "Objects/GameObject.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	MeshRender::MeshRender() :
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler())
	{
	}

	MeshRender::~MeshRender()
	{
	}

	void MeshRender::Start()
	{
	}

	void MeshRender::Update()
	{
		auto material = GetGameObject()->GetComponent<IMaterial>();

		if (material == nullptr)
		{
			return;
		}

		// Updates uniforms.
		material->PushUniforms(m_uniformObject);
	}

	void MeshRender::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const GraphicsStage &graphicsStage)
	{
		// Checks if the mesh is in view.
		/*auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr)
		{
			if (!shape->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

		// Gets required components.
		auto material = GetGameObject()->GetComponent<IMaterial>();
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (material == nullptr || mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		if (material->GetMaterial()->GetPipeline().GetGraphicsStage() != graphicsStage)
		{
			return;
		}

		// Binds the material pipeline.
		material->GetMaterial()->GetPipeline().BindPipeline(commandBuffer);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		material->PushDescriptors(m_descriptorSet);
		bool updateSuccess = m_descriptorSet.Update(material->GetMaterial()->GetPipeline());

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}

	void MeshRender::Decode(const Metadata &metadata)
	{
	}

	void MeshRender::Encode(Metadata &metadata) const
	{
	}

	bool MeshRender::operator<(const MeshRender &other) const
	{
		auto camera = Scenes::Get()->GetCamera();

		float thisDistance2 = (camera->GetPosition() - GetGameObject()->GetTransform().GetPosition()).LengthSquared();
		float otherDistance2 = (camera->GetPosition() - other.GetGameObject()->GetTransform().GetPosition()).LengthSquared();

		return thisDistance2 > otherDistance2;
	}
}
