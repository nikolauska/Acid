#include "MaterialDefault.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Models/VertexModel.hpp"
#include "Objects/GameObject.hpp"

namespace acid
{
	MaterialDefault::MaterialDefault(const Colour &baseDiffuse, const std::shared_ptr<Texture> &diffuseTexture,
									 const float &metallic, const float &roughness, const std::shared_ptr<Texture> &materialTexture, const std::shared_ptr<Texture> &normalTexture,
									 const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		m_animated(false),
		m_baseDiffuse(baseDiffuse),
		m_diffuseTexture(diffuseTexture),
		m_metallic(metallic),
		m_roughness(roughness),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_material(nullptr)
	{
	}

	MaterialDefault::~MaterialDefault()
	{
	}

	void MaterialDefault::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh == nullptr)
		{
			Log::Error("Cannot have a material attached to a object without a mesh!\n");
			return;
		}

		m_animated = std::dynamic_pointer_cast<MeshAnimated>(mesh) != nullptr;
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"},
			mesh->GetVertexInput(), PIPELINE_MODE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, GetDefines()));
	}

	void MaterialDefault::Update()
	{
	}

	void MaterialDefault::Decode(const Metadata &metadata)
	{
		m_baseDiffuse = metadata.GetChild<Colour>("Base Diffuse");
		TrySetDiffuseTexture(metadata.GetChild<std::string>("Diffuse Texture"));

		m_metallic = metadata.GetChild<float>("Metallic");
		m_roughness = metadata.GetChild<float>("Roughness");
		TrySetMaterialTexture(metadata.GetChild<std::string>("Material Texture"));
		TrySetNormalTexture(metadata.GetChild<std::string>("Normal Texture"));

		m_castsShadows = metadata.GetChild<bool>("Casts Shadows");
		m_ignoreLighting = metadata.GetChild<bool>("Ignore Lighting");
		m_ignoreFog = metadata.GetChild<bool>("Ignore Fog");
	}

	void MaterialDefault::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Colour>("Base Diffuse", m_baseDiffuse);
		metadata.SetChild<std::string>("Diffuse Texture", m_diffuseTexture == nullptr ? "" : m_diffuseTexture->GetFilename());

		metadata.SetChild<float>("Metallic", m_metallic);
		metadata.SetChild<float>("Roughness", m_roughness);
		metadata.SetChild<std::string>("Material Texture", m_materialTexture == nullptr ? "" : m_materialTexture->GetFilename());
		metadata.SetChild<std::string>("Normal Texture", m_normalTexture == nullptr ? "" : m_normalTexture->GetFilename());

		metadata.SetChild<bool>("Casts Shadows", m_castsShadows);
		metadata.SetChild<bool>("Ignore Lighting", m_ignoreLighting);
		metadata.SetChild<bool>("Ignore Fog", m_ignoreFog);
	}

	void MaterialDefault::PushUniforms(UniformHandler &uniformObject)
	{
		if (m_animated)
		{
			auto meshAnimated = GetGameObject()->GetComponent<MeshAnimated>();
			auto joints = meshAnimated->GetJointTransforms();
			uniformObject.Push("jointTransforms", *joints.data(), sizeof(Matrix4) * joints.size());
		}

		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("baseDiffuse", m_baseDiffuse);
		uniformObject.Push("metallic", m_metallic);
		uniformObject.Push("roughness", m_roughness);
		uniformObject.Push("ignoreFog", static_cast<float>(m_ignoreFog));
		uniformObject.Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
	}

	void MaterialDefault::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerDiffuse", m_diffuseTexture);
		descriptorSet.Push("samplerMaterial", m_materialTexture);
		descriptorSet.Push("samplerNormal", m_normalTexture);
	}

	std::vector<PipelineDefine> MaterialDefault::GetDefines()
	{
		std::vector<PipelineDefine> result = {};

		if (m_diffuseTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("DIFFUSE_MAPPING", "TRUE"));
		}

		if (m_materialTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("MATERIAL_MAPPING", "TRUE"));
		}

		if (m_animated)
		{
			result.emplace_back(PipelineDefine("ANIMATED", "TRUE"));
			result.emplace_back(PipelineDefine("MAX_JOINTS", std::to_string(MeshAnimated::MAX_JOINTS)));
			result.emplace_back(PipelineDefine("MAX_WEIGHTS", std::to_string(MeshAnimated::MAX_WEIGHTS)));
		}

		/*if (m_normalTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("NORMAL_MAPPING", "TRUE"));
		}*/

		return result;
	}
}
