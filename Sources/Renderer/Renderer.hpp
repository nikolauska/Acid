#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Commands/CommandBuffer.hpp"
#include "Engine/Engine.hpp"
#include "Swapchain/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "IManagerRender.hpp"
#include "RenderStage.hpp"

namespace acid
{
	class ACID_EXPORT Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		std::vector<std::shared_ptr<RenderStage>> m_renderStages;

		std::shared_ptr<Swapchain> m_swapchain;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchainImage;

		VkPipelineCache m_pipelineCache;

		VkSemaphore m_semaphore;
		VkCommandPool m_commandPool;

		std::shared_ptr<CommandBuffer> m_commandBuffer;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static std::shared_ptr<Renderer> Get() { return Engine::Get()->GetModule<Renderer>(); }

		Renderer();

		~Renderer();

		void Update() override;

		void CreateRenderpass(const std::vector<RenderpassCreate> &renderpassCreates);

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into a image file.
		/// </summary>
		///	<param name="filename"> The file to save the screenshot to. </param>
		void CaptureScreenshot(const std::string &filename);

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		IManagerRender *GetManager() const { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(IManagerRender *managerRender)
		{
			delete m_managerRender; // TODO: Cleanup.
			m_managerRender = managerRender;
		}

		std::vector<std::shared_ptr<RenderStage>> GetRenderStages() const { return m_renderStages; }

		std::shared_ptr<RenderStage> GetRenderStage(const uint32_t &index) const { return m_renderStages.at(index); }

		std::shared_ptr<Swapchain> GetSwapchain() const { return m_swapchain; }

		VkCommandPool GetCommandPool() const { return m_commandPool; }

		std::shared_ptr<CommandBuffer> GetCommandBuffer() const { return m_commandBuffer; }

		uint32_t GetActiveSwapchainImage() const { return m_activeSwapchainImage; }

		VkPipelineCache GetPipelineCache() const { return m_pipelineCache; }
	private:
		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreatePass(const uint32_t &i);

		bool StartRenderpass(const uint32_t &i);

		void EndRenderpass(const uint32_t &i);

		void NextSubpass();
	};
}
