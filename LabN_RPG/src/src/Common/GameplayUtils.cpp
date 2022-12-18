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
}