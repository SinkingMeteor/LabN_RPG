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
	}

	void GameWorld::InitializeSystems()
	{
		m_systems.emplace_back(std::make_unique<PlayerControllerSystem>(this));
		m_systems.emplace_back(std::make_unique<ActorMovementSystem>(this));

		m_systems.emplace_back(std::make_unique<AnimationStateSystem>(this));
		m_systems.emplace_back(std::make_unique<AnimationSystem>(this));

		m_systems.emplace_back(std::make_unique<CameraFollowingSystem>(this));
		m_systems.emplace_back(std::make_unique<ApplyTransformSystem>(this));

		m_renderSystems.emplace_back(std::make_unique<SpriteRenderSystem>());
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

		auto view = m_registry.view<TransformComponent, DrawableComponent>();
		for (entt::entity entity : view)
		{
			TransformComponent& heroTransform = view.get<TransformComponent>(entity);
			DrawableComponent& heroDrawable = view.get<DrawableComponent>(entity);
			for (int rectIndex : heroDrawable.RectsIndices)
			{
				if (rectIndex < 0) continue;
				TextureRect rect = heroDrawable.RelatedTexture->RectDatas[rectIndex];

				sf::CircleShape circle{ 1.0f };
				circle.setFillColor(sf::Color::Magenta);
				circle.setPosition(heroTransform.GlobalTransform * rect.Pivot);
				window.draw(circle);
			}
		}

		m_window->SetView(m_window->GetView());
		window.display();
	}

	void GameWorld::LoadResources()
	{
		std::ifstream input{ static_cast<const char*>("./resources/Worlds/World_Forest.json")};
		nlohmann::json rootNode;
		input >> rootNode;
		input.close();

		assert(!rootNode.is_null());

		nlohmann::json& mapNode = rootNode["map"];

		MapLoadingData mapLoadingData{ mapNode["path"], mapNode["fileName"]};
		m_mapFactory.LoadMap(m_registry, mapLoadingData, m_rootEntity);

		entt::entity playerController = m_registry.create();
		m_registry.emplace<PlayerControllerComponent>(playerController);

		nlohmann::json& actorsNode = rootNode["actors"];

		for (const nlohmann::json& actorData : actorsNode)
		{
			std::optional<entt::entity> actor = m_actorFactory.CreateEntity(m_registry, actorData, m_rootEntity);
		}
	}
}