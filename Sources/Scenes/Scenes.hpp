#pragma once

#include "Engine/Engine.hpp"
#include "Objects/ComponentRegister.hpp"
#include "IScene.hpp"
#include "SceneStructure.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class ACID_EXPORT Scenes :
		public IModule
	{
	private:
		IScene *m_scene;

		ComponentRegister m_componentRegister;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static std::shared_ptr<Scenes> Get() { return Engine::Get()->GetModule<Scenes>(); }

		/// <summary>
		/// Creates a new Scenes module.
		/// </summary>
		Scenes();

		~Scenes();

		void Update() override;

		/// <summary>
		/// Gets the current scene.
		/// </summary>
		/// <returns> The current scene. </returns>
		IScene *GetScene() const { return m_scene; }

		/// <summary>
		/// Sets the current scene to a new scene.
		/// </summary>
		/// <param name="scene"> The new scene. </param>
		void SetScene(IScene *scene)
		{
			delete m_scene; // TODO: Cleanup.
			m_scene = scene;
		}

		/// <summary>
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		std::shared_ptr<IComponent> CreateComponent(const std::string &name) { return m_componentRegister.CreateComponent(name); }

		/// <summary>
		/// Registers a component with the register.
		/// </summary>
		/// <param name="update"> The components update type. </param>
		/// <param name="name"> The components name. </param>
		/// <param name="T"> The components type. </param>
		template<typename T>
		void RegisterComponent(const std::string &name) { m_componentRegister.RegisterComponent<T>(name); }

		/// <summary>
		/// Deregisters a component.
		/// </summary>
		/// <param name="name"> The components name. </param>
		/// <returns> If the component was deregistered. </returns>
		bool DeregisterComponent(const std::string &name) { return m_componentRegister.DeregisterComponent(name); }

		/// <summary>
		/// Finds the registered name to a component.
		/// </summary>
		/// <param name="compare"> The components to get the registered name of. </param>
		/// <returns> The name registered to the component. </returns>
		std::optional<std::string> FindComponentName(const std::shared_ptr<IComponent> &compare) { return m_componentRegister.FindComponentName(compare); }

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		ICamera *GetCamera() const { return m_scene->GetCamera(); }

		/// <summary>
		/// Gets the scene physics system.
		/// </summary>
		/// <returns> The scenes physics system. </returns>
		std::unique_ptr<ScenePhysics> const &GetPhysics() const { return m_scene->GetPhysics(); }

		/// <summary>
		/// Gets the scene object structure.
		/// </summary>
		/// <returns> The scene object structure. </returns>
		std::unique_ptr<SceneStructure> const &GetStructure() const { return m_scene->GetStructure(); }

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		bool IsGamePaused() const { return m_scene->IsGamePaused(); }
	};
}
