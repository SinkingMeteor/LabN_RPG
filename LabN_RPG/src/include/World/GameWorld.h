#pragma once
#include <iostream>
#include <cassert>

#include "World/World.h"
#include "Locator.h"
#include "Systems/ISystem.h"
#include "Systems/SpriteRenderSystem.h"
#include "entt/entt.hpp"
#include "Prefabs/ActorPrefab.h"
#include "Systems/ActorMovementSystem.h"
namespace vg
{
	class GameWorld : public World 
	{
	public:
		GameWorld();
		virtual void Initialize() override;
		virtual void Tick(sf::Time deltaTime) override;
		virtual void Render() override;

	private:
		std::vector<std::unique_ptr<ISystem>> m_systems;
		std::vector<std::unique_ptr<IRenderSystem>> m_renderSystems;
		std::vector<sf::Texture> m_textures;
		void InitializeServices();
		void LoadResources();
		void InitializeSystems();
	};
}