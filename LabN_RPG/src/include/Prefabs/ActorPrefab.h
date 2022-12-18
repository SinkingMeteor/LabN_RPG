#pragma once
#include "entt/entt.hpp"
#include "Components/Components.h"
#include "ResourceManagement/AnimationProvider.h"
#include "ResourceManagement/TextureProvider.h"
#include "SFML/Graphics.hpp"
#include "Database.h"
namespace vg 
{
	struct ActorLoadingData 
	{
	public:
		int TextureID;
		int AnimationPackID;
	};

	class ActorFactory 
	{
	public:
		ActorFactory(TextureProvider* texProvider, AnimationProvider* animProvider) :
			m_textureProvider(texProvider),
			m_animationProvider(animProvider) {}
		entt::entity CreateEntity(entt::registry& registry, const ActorLoadingData& data);
	private:
		TextureProvider* m_textureProvider;
		AnimationProvider* m_animationProvider;
	};
}