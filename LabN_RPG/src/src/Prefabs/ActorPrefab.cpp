#include "Prefabs/ActorPrefab.h"

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

		registry.emplace<TransformComponent>(actor, startPosition, sf::Vector2f{ 1.0f, 1.0f }, 0.0f);
		registry.emplace<MovementComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 0.0f, 0.0f }, 100.0f);
		
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

		SpriteComponent& spriteComponent = registry.emplace<SpriteComponent>(actor);
		spriteComponent.Sprite.setTexture(actorTexture->Texture);
		spriteComponent.Sprite.getTextureRect();

		AnimationComponent& animComponent = registry.emplace<AnimationComponent>(actor);
		animComponent.CurrentAnimationPack = &actorAnimation;
		animComponent.CurrentAnimation = &animComponent.CurrentAnimationPack->Animations[0];
		animComponent.CurrentFrame = 0;
		animComponent.CurrentFrame = 0.0f;
		
		return actor;
	}
}