#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include "ResourceManagement/TextureProvider.h"
#include <vector>
#include <unordered_map>
namespace vg 
{
	struct PlaceholdersComponent 
	{
		std::unordered_map<entt::id_type, sf::Vector2f> SpawnPoints{};
	};
}