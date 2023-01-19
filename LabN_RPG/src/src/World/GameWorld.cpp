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
		//====
		//DEBUG: Отображение пивотов
		auto pivotView = m_registry.view<TransformComponent>();

		for (entt::entity entity : pivotView)
		{
			TransformComponent& transformComponent = m_registry.get<TransformComponent>(entity);
			sf::CircleShape circle{ 1.0f };
			circle.setPosition(transformComponent.GlobalTransform * VGMath::Zero);
			window.draw(circle);
		}

		//====
		//DEBUG: Отображение пространств
		WorldPartitionComponent& partitionComponent = m_registry.get<WorldPartitionComponent>(m_rootEntity);
		sf::Vector2<std::size_t> gridSize = partitionComponent.Grid.GetGridSize();
		float cellSize = partitionComponent.Grid.GetCellSize();
		auto actorView = m_registry.view<CameraTarget>();

		for (size_t i = 0; i < gridSize.x * gridSize.y; ++i)
		{
			std::size_t xPos = i % gridSize.x;
			std::size_t yPos = i / gridSize.x;

			sf::Vector2f rectPosition = sf::Vector2f{ (float)cellSize * (float)xPos, (float)cellSize * (float)yPos };
			sf::RectangleShape rect{ sf::Vector2f{cellSize, cellSize} };
			rect.setPosition(rectPosition);
			rect.setFillColor(sf::Color::Transparent);

			PartitionCell& cell = partitionComponent.Grid.GetCell(rectPosition);

			for (entt::entity actor : actorView) 
			{
				if (cell.Contains(actor))
				{
					rect.setOutlineColor(sf::Color::Cyan);
				}
				else
				{
					rect.setOutlineColor(sf::Color::Magenta);
				}
			}

			rect.setOutlineThickness(-2.0f);
			window.draw(rect);
		}
		//====

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
		m_mapFactory.LoadMap(this, m_registry, mapLoadingData, m_rootEntity);

		entt::entity playerController = m_registry.create();
		m_registry.emplace<PlayerControllerComponent>(playerController);

		nlohmann::json& actorsNode = rootNode["actors"];

		for (const nlohmann::json& actorData : actorsNode)
		{
			std::optional<entt::entity> actor = m_actorFactory.CreateEntity(this, m_registry, actorData, m_rootEntity);
		}
	}
}