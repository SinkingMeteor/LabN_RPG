#pragma once 
#include "entt/entt.hpp"
#include "entt/core/hashed_string.hpp"
#include "SFML/Graphics.hpp"
#include "MathUtils.h"
#include "ResourceManagement/TextureProvider.h"
#include "Components/Components.h"
#include <string>
namespace vg 
{
	class CommonUtils 
	{
	public:
		static entt::id_type GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction);
		static void SetInitialPositionAndTexCoords(sf::VertexArray& vertexArray, const TextureRect& rect, const TransformComponent& transformComponent);
		static entt::id_type StringToId(const std::string& stringToConvert);
	};
}