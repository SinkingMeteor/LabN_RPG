#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
namespace vg 
{
	struct MapComponent 
	{
		std::vector<unsigned int> MapIndices{};
		sf::VertexArray VertexArray{};
		sf::Texture* RelatedTexture{};
		unsigned int MapWidth{};
		unsigned int MapHeight{};
		unsigned int TileSizeX{};
		unsigned int TileSizeY{};
	};
}