#pragma once
#include "vgpch.h"
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