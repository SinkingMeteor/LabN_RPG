#include "GameplayUtils.h"
#include <iostream>
namespace vg 
{
	entt::id_type GameplayUtils::GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction)
	{
        sf::Vector2f normalizedDirection = MathUtils::NormalizeVector2<float>(direction);
        float movesToRight = MathUtils::Dot(MathUtils::Right, normalizedDirection);

        float dotProduct = MathUtils::Dot(MathUtils::Up, normalizedDirection);
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

    void GameplayUtils::SetInitialPositionAndTexCoords(sf::VertexArray& quad, const TextureRect& spriteRect)
    {
        quad[0].position = -spriteRect.Pivot;
        quad[1].position = -spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
        quad[2].position = -spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
        quad[3].position = -spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };

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