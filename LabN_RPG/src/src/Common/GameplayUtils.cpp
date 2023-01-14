#include "GameplayUtils.h"
#include <iostream>
namespace vg 
{
	entt::id_type GameplayUtils::GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction)
	{
        sf::Vector2f normalizedDirection = VGMath::NormalizeVector2<float>(direction);
        float movesToRight = VGMath::Dot(VGMath::Right, normalizedDirection);

        float dotProduct = VGMath::Dot(VGMath::Up, normalizedDirection);
        std::string totalText { textWithNoPrefix };

        if (normalizedDirection.x > 0.0f) 
        {
            return entt::hashed_string{ (totalText + "D").c_str() }.value();
        }
        if (normalizedDirection.x < 0.0f)
        {
            return entt::hashed_string{ (totalText + "A").c_str() }.value();
        }
        if (normalizedDirection.y < 0.0f)
        {
            return entt::hashed_string{ (totalText + "W").c_str() }.value();
        }

        entt::id_type downId = entt::hashed_string{ (totalText + "S").c_str() }.value();
        return downId;
	}

    void GameplayUtils::SetInitialPositionAndTexCoords(sf::VertexArray& quad, const TextureRect& spriteRect, const TransformComponent& transformComponent)
    {
        sf::Vector2f spritePosition = transformComponent.LocalTransform * spriteRect.Pivot;
        quad[0].position = spritePosition - spriteRect.Pivot;
        quad[1].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
        quad[2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
        quad[3].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };

        float top = (float)spriteRect.Rect.top;
        float left = (float)spriteRect.Rect.left;
        float width = (float)spriteRect.Rect.width;
        float height = (float)spriteRect.Rect.height;

        quad[0].texCoords = sf::Vector2f{ left, top };
        quad[1].texCoords = sf::Vector2f{ left + width, top };
        quad[2].texCoords = sf::Vector2f{ left + width, top + height };
        quad[3].texCoords = sf::Vector2f{ left, top + height };
    }

}