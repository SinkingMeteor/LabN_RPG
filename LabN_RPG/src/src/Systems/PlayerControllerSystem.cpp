#include "Systems/PlayerControllerSystem.h"

namespace vg 
{
	void PlayerControllerSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<PlayerControllerComponent>();

		Keyboard& keyboard = Locator::Keyboard::value();

		sf::Vector2f velocityDirection{};

		if (keyboard.KeyIsPressed(sf::Keyboard::W))
		{
			velocityDirection.y -= 1.0f;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::A))
		{
			velocityDirection.x -= 1.0f;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::S))
		{
			velocityDirection.y += 1.0f;
		}
		if (keyboard.KeyIsPressed(sf::Keyboard::D))
		{
			velocityDirection.x += 1.0f;
		}


		for (entt::entity entity : view)
		{
			PlayerControllerComponent& pcComponent = view.get<PlayerControllerComponent>(entity);

			if (!registry.all_of<Possessable, MovementComponent>(pcComponent.PossesedEntity)) continue;

			MovementComponent& movementComponent = registry.get<MovementComponent>(pcComponent.PossesedEntity);
			movementComponent.Velocity = velocityDirection;
		}
	}
}