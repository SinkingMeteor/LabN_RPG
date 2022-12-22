#pragma once 
#include "SFML/Graphics.hpp"
#include "ResourceManagement/TextureProvider.h"
#include "entt/entt.hpp"
#include "Components/Components.h"
#include <fstream>
#include <optional>
#include <utility>
#include "json.hpp"
#include <string>
namespace vg
{
	struct MapLoadingData 
	{
		const std::string LoadingPath;
		const std::string MapFileName;
	};

	class MapFactory
	{
	public:
		MapFactory(TextureProvider* texProvider) :
			m_textureProvider(texProvider)
		{}
		std::optional<entt::entity> CreateEntity(entt::registry& registry, const MapLoadingData& data);
	private:
		TextureProvider* m_textureProvider;
	};
}