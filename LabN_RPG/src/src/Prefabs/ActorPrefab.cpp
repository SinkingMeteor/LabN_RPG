#include "Prefabs/ActorPrefab.h"
#include "Common/GameplayUtils.h"
namespace vg 
{
	std::optional<entt::entity> ActorFactory::CreateEntity(entt::registry& registry, const nlohmann::json& data)
	{
		assert(!data.is_null());

		auto placeholdersView = registry.view<PlaceholdersComponent>();
		assert(placeholdersView.size() == 1);

		entt::entity mapEntity = *placeholdersView.begin();
		PlaceholdersComponent& placeholdersComponent = registry.get<PlaceholdersComponent>(mapEntity);
		
		entt::id_type actorNameId = entt::hashed_string{ data["name"].get<std::string>().c_str() }.value();
		std::string texturePath = data["spriteFilePath"];
		std::string animationPath = data["animationFilePath"];

		auto heroTexResult = m_textureProvider->load(actorNameId, texturePath);
		SlicedTexture* actorTexture = &*(*m_textureProvider)[actorNameId];
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

		TransformComponent& transformComponent = registry.emplace<TransformComponent>(actor);
		transformComponent.Origin = spriteRect.Pivot;
		transformComponent.Transform.translate(startPosition);
		transformComponent.Transform.scale(sf::Vector2f{ 1.0f, 1.0f });

		GameplayUtils::SetInitialPositionAndTexCoords(quad, spriteRect, transformComponent);

		DrawableComponent& spriteComponent = registry.emplace<DrawableComponent>(actor);
		spriteComponent.VertexArray = std::move(quad);
		spriteComponent.RectsIndices.push_back(0);
		spriteComponent.RelatedTexture = actorTexture;
		spriteComponent.SpriteWidthByTiles = 1;
		spriteComponent.SpriteHeightByTiles = 1;

		AnimationComponent& animComponent = registry.emplace<AnimationComponent>(actor);
		animComponent.CurrentAnimationPack = &actorAnimation;
		animComponent.CurrentAnimation = &animComponent.CurrentAnimationPack->Animations[0];
		animComponent.CurrentFrame = 0;
		animComponent.CurrentFrame = 0.0f;
		
		return actor;
	}
}