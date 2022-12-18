#include "Prefabs/ActorPrefab.h"

namespace vg 
{
	entt::entity ActorFactory::CreateEntity(entt::registry& registry, const ActorLoadingData& data)
	{
		assert(m_textureProvider->contains(data.TextureID));
		assert(m_animationProvider->contains(data.AnimationPackID));

		entt::entity actor = registry.create();

		registry.emplace<TransformComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 1.0f, 1.0f }, 0.0f);
		registry.emplace<MovementComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, 100.0f);
		registry.emplace<Possessable>(actor);

		TextureResource result = (*m_textureProvider)[data.TextureID];
		registry.emplace<SpriteComponent>(actor).Sprite.setTexture(*result);

		AnimationResource animResource = (*m_animationProvider)[data.AnimationPackID];
		AnimationComponent& animComponent = registry.emplace<AnimationComponent>(actor);
		animComponent.CurrentAnimationPack = &*animResource;
		animComponent.CurrentAnimation = &animComponent.CurrentAnimationPack->Animations[Database::AnimTypes::IDLE_S];
		animComponent.CurrentFrame = 0;
		animComponent.CurrentFrame = 0.0f;
		
		return actor;
	}
}