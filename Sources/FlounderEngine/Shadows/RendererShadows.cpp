#include <Meshes/Mesh.hpp>
#include <Entities/EntityRender.hpp>
#include "RendererShadows.hpp"

#include "../Devices/Display.hpp"
#include "../Materials/Material.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosShadows.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_FRONT_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(0), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL) // uboObject
			}, // descriptors

			{"Resources/Shaders/Shadows/Shadow.vert.spv", "Resources/Shaders/Shadows/Shadow.frag.spv"} // shaderStages
		};

	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosShadows::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PIPELINE_CREATE))
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererShadows::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosShadows::UboScene uboScene = {};
		uboScene.projectionView = *Shadows::Get()->GetShadowBox()->GetProjectionViewMatrix();
		// uboScene.projectionView = *Scenes::Get()->GetCamera()->GetProjectionMatrix() * *Scenes::Get()->GetCamera()->GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<Mesh *> meshList = std::vector<Mesh *>();
		Scenes::Get()->GetStructure()->QueryComponents(&meshList);

		for (auto mesh : meshList)
		{
			if (mesh->GetModel() == nullptr)
			{
				continue;
			}

			auto material = mesh->GetGameObject()->GetComponent<Material>();

			if (material == nullptr || !material->GetCastsShadows())
			{
				continue;
			}

			RenderModel(commandBuffer, mesh->GetModel(), mesh->GetGameObject());
		}
	}

	void RendererShadows::RenderModel(const VkCommandBuffer &commandBuffer, Model *object, GameObject *gameObject)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		auto entityRender = gameObject->GetComponent<EntityRender>();
		if (entityRender == nullptr)
			return;
		auto uniformObject = entityRender->GetUniformObject();

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			uniformObject->GetWriteDescriptor(1, descriptorSet)
		};

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		// Draws the object.
		VkDescriptorSet descriptors[] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		object->CmdRender(commandBuffer);
	}
}
