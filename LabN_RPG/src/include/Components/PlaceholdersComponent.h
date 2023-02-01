#pragma once
#include "vgpch.h"
#include "ResourceManagement/TextureProvider.h"

namespace vg 
{
	struct PlaceholdersComponent 
	{
		std::unordered_map<entt::id_type, sf::Vector2f> SpawnPoints{};
	};
}