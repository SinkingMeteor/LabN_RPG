#include "Prefabs/ActorPrefab.h"
#include "Common/GameplayUtils.h"
namespace vg 
{
	std::optional<entt::entity> ActorFactory::CreateEntity(World* world, entt::registry& registry, const nlohmann::json& data, entt::entity parent)
	{
		assert(world);
		assert(!data.is_null());
		assert(registry.all_of<NodeComponent>(parent) || registry.all_of<TransformComponent>(parent));

		auto placeholdersView = registry.view<PlaceholdersComponent>();
		assert(placeholdersView.size() == 1);

		entt::entity mapEntity = *placeholdersView.begin();
		PlaceholdersComponent& placeholdersComponent = registry.get<PlaceholdersComponent>(mapEntity);
		
		WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(world->GetSceneRootEntity());

		entt::id_type actorNameId = entt::hashed_string{ data["name"].get<std::string>().c_str() }.value();
		std::string texturePath = data["spriteFilePath"];
		std::string animationPath = data["animationFilePath"];

		auto heroTexResult = m_textureProvider->load(actorNameId, texturePath);
		SlicedTexture* actorTexture = (*m_textureProvider)[actorNameId].handle().get();
		m_animationProvider->load(actorNameId, AnimationLoadingData{ animationPath, actorTexture});
		AnimationPack& actorAnimation = *(*m_animationProvider)[actorNameId];

		entt::entity actor = registry.create();

		sf::Vector2f startPosition{};
		auto spawnPointIterator = placeholdersComponent.SpawnPoints.find(actorNameId);
		if (spawnPointIterator != placeholdersComponent.SpawnPoints.end()) 
		{
			startPosition = spawnPointIterator->second;
		}

		registry.emplace<MovementComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 0.0f, 0.0f }, 100.0f);
		registry.emplace<OnGroundSortingLayer>(actor);
		
		bool isPlayable = data["isPlayable"].get<bool>();

		registry.emplace<Possessable>(actor);
		if (isPlayable) 
		{
			registry.emplace<CameraTarget>(actor);
			auto view = registry.view<PlayerControllerComponent>();
			for (entt::entity entity : view)
			{
				PlayerControllerComponent& playerController = view.get<PlayerControllerComponent>(entity);
				playerController.PossesedEntity = actor;
			}
		}

		TextureRect& spriteRect = actorTexture->RectDatas[0];
		sf::VertexArray quad( sf::Quads, 4 );

		TransformComponent& parentTransformComponent = registry.get<TransformComponent>(parent);
		registry.get<NodeComponent>(parent).Children.push_back(actor);
		NodeComponent& nodeComponent = registry.emplace<NodeComponent>(actor);
		nodeComponent.Parent = parent;

		TransformComponent& transformComponent = registry.emplace<TransformComponent>(actor);
		transformComponent.GlobalTransform.translate(startPosition);
		transformComponent.LocalTransform.translate(parentTransformComponent.GlobalTransform.getInverse() * startPosition);
		transformComponent.LocalTransform.scale(sf::Vector2f{ 1.0f, 1.0f });

		PartitionCell& cell = partitionGrid.Grid.GetCell(startPosition);
		cell.AddEntity(actor);

		DrawableComponent& spriteComponent = registry.emplace<DrawableComponent>(actor);
		spriteComponent.VertexArray = std::move(quad);
		spriteComponent.RelatedTexture = actorTexture;
		spriteComponent.RectIndex = 0;

		AnimationComponent& animComponent = registry.emplace<AnimationComponent>(actor);
		animComponent.CurrentAnimationPack = &actorAnimation;
		animComponent.CurrentAnimation = &animComponent.CurrentAnimationPack->Animations[0];
		animComponent.CurrentFrame = 0;
		animComponent.CurrentFrame = 0.0f;
		
		return actor;
	}
}