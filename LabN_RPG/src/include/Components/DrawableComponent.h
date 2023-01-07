#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManagement/TextureProvider.h"
namespace vg 
{
	struct DrawableComponent 
	{
	public:
		sf::VertexArray VertexArray{};
		std::vector<int> RectsIndices{};
		SlicedTexture* RelatedTexture{};
		std::size_t SpriteWidthByTiles{};
		std::size_t SpriteHeightByTiles{};
	};
}