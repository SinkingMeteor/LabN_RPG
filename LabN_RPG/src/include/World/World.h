#pragma once 
#include "vgpch.h"
#include "Window/Window.h"
namespace vg 
{
	class World 
	{
	public:
		World(Window* window);
		Window* GetWindow() { return m_window; }
		entt::entity GetSceneRootEntity() const { return m_rootEntity; }
		
		const std::unordered_map<entt::id_type, entt::entity>& GetUniqueObjectsMap() const { return m_uniqueObjects; }
		void AddUniqueObject(entt::id_type id, entt::entity entity);
		void RemoveUniqueObject(entt::id_type id);
		bool ContainsUniqueObject(entt::id_type id);

		virtual ~World() {};
		virtual void Initialize() = 0;
		virtual void Tick(sf::Time deltaTime) = 0;
		virtual void Render() = 0;

	protected:
		Window* m_window;
		entt::registry m_registry;
		entt::entity m_rootEntity;

		std::unordered_map<entt::id_type, entt::entity> m_uniqueObjects;
	};
}