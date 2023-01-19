#pragma once
#include "entt/entt.hpp"
#include "Components/Components.h"
#include "ResourceManagement/AnimationProvider.h"
#include "ResourceManagement/TextureProvider.h"
#include "SFML/Graphics.hpp"
#include <optional>
#include <string>
#include "World/World.h"
#include "Database.h"
namespace vg 
{
	struct ActorLoadingData 
	{
		const std::string Name;
		const std::string TextureFullPath;
		const std::string AnimationFullPath;
	};

	class ActorFactory 
	{
	public:
		ActorFactory(TextureProvider* texProvider, AnimationProvider* animProvider) :
			m_textureProvider(texProvider),
			m_animationProvider(animProvider) {}
		std::optional<entt::entity> CreateEntity(World* world, entt::registry& registry, const nlohmann::json& dataNode, entt::entity parent);
	private:
		TextureProvider* m_textureProvider;
		AnimationProvider* m_animationProvider;
	};
}