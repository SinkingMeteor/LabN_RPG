#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include <vector>
#include <unordered_map>
namespace vg 
{
	struct MapComponent 
	{
		std::unordered_map<entt::id_type, sf::Vector2f> SpawnPoints{};
		std::vector<unsigned int> MapIndices{};
		sf::VertexArray VertexArray{};
		sf::Texture* RelatedTexture{};
		sf::Vector2<unsigned int> MapSize{};
		sf::Vector2<unsigned int> TileSize{};
	};
}