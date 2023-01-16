#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManagement/TextureProvider.h"
namespace vg 
{

	struct DrawableComponent 
	{
	public:
		sf::VertexArray VertexArray{};
		SlicedTexture* RelatedTexture{};
		int RectIndex{};
	};
}