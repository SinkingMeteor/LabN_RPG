#include "Systems/AnimationSystem.h"
#include <cmath>
namespace vg
{
	void AnimationSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<AnimationComponent>();

		for (entt::entity entity : view)
		{
			AnimationComponent& animationComponent = view.get<AnimationComponent>(entity);

			if (!animationComponent.CurrentAnimation) continue;

			Animation* currentAnimation = animationComponent.CurrentAnimation;
			std::size_t framesCount = currentAnimation->Frames.size();

			animationComponent.CurrentFrame += deltaTime.asSeconds() * animationComponent.CurrentAnimation->FrameRate;

			if (animationComponent.CurrentFrame >= framesCount) 
			{
				//std::cout << "Was added " << deltaTime.asSeconds() * animationComponent.CurrentAnimation->FrameRate << '\n';
				//std::cout << "Anim frame size: " << framesCount << " Current frame: " << animationComponent.CurrentFrame << '\n';
				animationComponent.CurrentFrame = fmod(animationComponent.CurrentFrame, framesCount);
			}
			animationComponent.CurrentIndex = static_cast<int>(animationComponent.CurrentFrame);
		}
	}
}