#include "World/GameWorld.h"

namespace vg 
{
	GameWorld::GameWorld() :
		m_systems(),
		m_renderSystems(),
		m_textures()
	{}

	void GameWorld::Initialize()
	{
		InitializeServices();
		LoadResources();
		InitializeSystems();

		entt::registry& registry = Locator::ECS::value();
		entt::entity hero = ActorPrefab::CreateEntity(registry);
		SpriteComponent& spriteComponent = registry.get<SpriteComponent>(hero);
		spriteComponent.Sprite.setTexture(m_textures.at(0));
	}

	void GameWorld::Tick(sf::Time deltaTime)
	{
		assert(Locator::ECS::has_value());

		entt::registry& registry = Locator::ECS::value();
		for (std::unique_ptr<ISystem>& systemPointer : m_systems) 
		{
			systemPointer->Tick(registry, deltaTime);
		}
	}

	void GameWorld::Render()
	{
		assert(Locator::ECS::has_value());
		assert(Locator::GameWindow::has_value());

		entt::registry& registry = Locator::ECS::value();
		sf::RenderWindow& window = Locator::GameWindow::value().GetWindow();

		window.clear(sf::Color::Black);

		for (std::unique_ptr<IRenderSystem>& systemPointer : m_renderSystems)
		{
			systemPointer->Render(registry, window);
		}

		window.display();
	}

	void GameWorld::InitializeServices()
	{
		if (Locator::ECS::has_value())
		{
			Locator::ECS::reset();
		}
		Locator::ECS::emplace();
	}

	void GameWorld::LoadResources()
	{
		sf::Texture tex{};
		if (!tex.loadFromFile("D:/Projects/VisualStudio/LabN_RPG/LabN_RPG/resources/Textures/Hero.png")) 
		{
			std::cout << "Path not found!" << std::endl;
			return;
		}
		m_textures.emplace_back<sf::Texture>(std::move(tex));
	}

	void GameWorld::InitializeSystems()
	{
		m_renderSystems.emplace_back(std::make_unique<SpriteRenderSystem>());
	}
}