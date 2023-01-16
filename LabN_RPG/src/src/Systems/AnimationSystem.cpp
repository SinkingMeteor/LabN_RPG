#include "Systems/AnimationSystem.h"
#include "Components/Components.h"
#include <cmath>
namespace vg
{
	void AnimationSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<TransformComponent, DrawableComponent, AnimationComponent>();

		for (entt::entity entity : view)
		{
			//TransformComponent& transformCompo
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			AnimationComponent& animationComponent = view.get<AnimationComponent>(entity);

			if (!animationComponent.CurrentAnimation) continue;

			Animation* currentAnimation = animationComponent.CurrentAnimation;
			std::size_t framesCount = currentAnimation->Frames.size();

			animationComponent.CurrentFrame += deltaTime.asSeconds() * animationComponent.CurrentAnimation->FrameRate;

			if (animationComponent.CurrentFrame >= framesCount) 
			{
				animationComponent.CurrentFrame = fmod(animationComponent.CurrentFrame, framesCount);
			}
			animationComponent.CurrentIndex = static_cast<int>(animationComponent.CurrentFrame);

			const std::size_t frameIndex = animationComponent.CurrentAnimation->Frames[animationComponent.CurrentFrame];
			spriteComponent.RectIndex = frameIndex;
		}
	}
}