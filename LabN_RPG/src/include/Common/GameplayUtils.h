#pragma once 
#include "vgpch.h"
#include "MathUtils.h"
#include "ResourceManagement/TextureProvider.h"
#include "Components/Components.h"

namespace vg 
{
	class VGUtils 
	{
	public:
		static entt::id_type GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction);
		static void SetInitialPositionAndTexCoords(sf::VertexArray& vertexArray, const TextureRect& rect, const TransformComponent& transformComponent);
		static entt::id_type StringToId(const std::string& stringToConvert);
		static void DetachFromParent(entt::registry& registry, entt::entity entity);
		static void AttachTo(entt::registry& registry, entt::entity childEntity, entt::entity parentEntity);

		template<typename... Args>
		static void Log(const char* logScope, const char* format, Args&&...args)
		{
			printf(logScope);
			printf(format, std::forward<Args>(args)...);
			printf("\n");
		}
	};
}