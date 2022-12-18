#include "Systems/PlayerControllerSystem.h"

namespace vg 
{
	void PlayerControllerSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<PlayerControllerComponent>();

		Keyboard& keyboard = m_world->GetWindow()->GetKeyboard();

		sf::Vector2f velocityDirection{};
		entt::id_type CurrentAnimationId{};

		if (keyboard.KeyIsPressed(sf::Keyboard::W))
		{
			velocityDirection.y -= 1.0f;
			CurrentAnimationId = Database::AnimTypes::IDLE_W;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::A))
		{
			velocityDirection.x -= 1.0f;
			CurrentAnimationId = Database::AnimTypes::IDLE_A;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::S))
		{
			velocityDirection.y += 1.0f;
			CurrentAnimationId = Database::AnimTypes::IDLE_S;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::D))
		{
			velocityDirection.x += 1.0f;
			CurrentAnimationId = Database::AnimTypes::IDLE_D;
		}


		for (entt::entity entity : view)
		{
			PlayerControllerComponent& pcComponent = view.get<PlayerControllerComponent>(entity);

			if (!registry.all_of<Possessable, MovementComponent, AnimationComponent>(pcComponent.PossesedEntity)) continue;

			AnimationComponent& animationComponent = registry.get<AnimationComponent>(pcComponent.PossesedEntity);
			if (CurrentAnimationId != 0 && CurrentAnimationId != animationComponent.CurrentAnimation->AnimationId)
			{
				Animation* newAnimation = &animationComponent.CurrentAnimationPack->Animations[CurrentAnimationId];
				animationComponent.CurrentFrame = 0.0f;
				animationComponent.CurrentIndex = 0;
				animationComponent.CurrentAnimation = newAnimation;
			}

			MovementComponent& movementComponent = registry.get<MovementComponent>(pcComponent.PossesedEntity);
			movementComponent.Velocity = velocityDirection;
		}
	}
}