#include "World/GameWorld.h"

namespace vg 
{
	GameWorld::GameWorld(Window* window) :
		World(window),
		m_systems(),
		m_renderSystems(),
		m_textureProvider(),
		m_animationProvider(),
		m_actorFactory(&m_textureProvider, &m_animationProvider),
		m_mapFactory(&m_textureProvider)
	{}

	void GameWorld::Initialize()
	{
		LoadResources();
		InitializeSystems();

		ActorLoadingData playerLoadingData{};
		playerLoadingData.TextureID = Database::Textures::HERO_ATLAS;
		playerLoadingData.AnimationPackID = Database::AnimConfigs::HERO_ANIM_CONFIG;
		std::optional<entt::entity> hero = m_actorFactory.CreateEntity(m_registry, playerLoadingData);
		m_registry.emplace<CameraTarget>(hero.value());

		entt::entity playerController = m_registry.create();
		m_registry.emplace<PlayerControllerComponent>(playerController, hero.value());

		AnimationResource animResult = m_animationProvider[Database::AnimConfigs::HERO_ANIM_CONFIG];
		std::cout << animResult->Animations.size() << '\n';
		std::cout << animResult->Animations[Database::AnimTypes::IDLE_W].Frames.at(0).top;

		MapLoadingData mapLoadingData{ "./resources/Maps/DesertMap.json", Database::Textures::DESERT_MAP};
		m_mapFactory.CreateEntity(m_registry, mapLoadingData);
	}

	void GameWorld::Tick(sf::Time deltaTime)
	{
		for (std::unique_ptr<System>& systemPointer : m_systems) 
		{
			systemPointer->Tick(m_registry, deltaTime);
		}
	}

	void GameWorld::Render()
	{
		sf::RenderWindow& window = m_window->GetWindow();

		window.clear(sf::Color::Black);

		for (std::unique_ptr<IRenderSystem>& systemPointer : m_renderSystems)
		{
			systemPointer->Render(m_registry, window);
		}

		m_window->SetView(m_worldView);
		window.display();
	}

	void GameWorld::LoadResources()
	{
		auto heroTexResult = m_textureProvider.load(entt::id_type{Database::Textures::HERO_ATLAS}, "./resources/Textures/Actors/Hero.png");
		auto desertMapResult = m_textureProvider.load(entt::id_type{Database::Textures::DESERT_MAP}, "./resources/Textures/Maps/DesertMap.png");

		sf::Texture* heroTexture = &*m_textureProvider[Database::Textures::HERO_ATLAS];
		m_animationProvider.load(entt::id_type{Database::AnimConfigs::HERO_ANIM_CONFIG}, AnimationLoadingData{ "./resources/Animations/Anim_Hero.json", heroTexture});
	}

	void GameWorld::InitializeSystems()
	{
		m_systems.emplace_back(std::make_unique<PlayerControllerSystem>(this));
		m_systems.emplace_back(std::make_unique<ActorMovementSystem>(this));

		m_systems.emplace_back(std::make_unique<AnimationStateSystem>(this));
		m_systems.emplace_back(std::make_unique<AnimationSystem>(this));
		
		m_systems.emplace_back(std::make_unique<CameraFollowingSystem>(this));

		m_renderSystems.emplace_back(std::make_unique<MapRenderSystem>());
		m_renderSystems.emplace_back(std::make_unique<SpriteRenderSystem>());
	}
}