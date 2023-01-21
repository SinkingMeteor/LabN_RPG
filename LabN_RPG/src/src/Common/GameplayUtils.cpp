#include "GameplayUtils.h"
#include <iostream>
namespace vg 
{
	entt::id_type VGUtils::GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction)
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

    void VGUtils::SetInitialPositionAndTexCoords(sf::VertexArray& quad, const TextureRect& spriteRect, const TransformComponent& transformComponent)
    {
        sf::Vector2f spritePosition = transformComponent.GlobalTransform * VGMath::Zero;

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

    entt::id_type VGUtils::StringToId(const std::string& stringToConvert)
    {
        return entt::hashed_string{ stringToConvert.c_str() }.value();
    }

    void VGUtils::DetachFromParent(entt::registry& registry, entt::entity entity)
    {
        assert(registry.all_of<NodeComponent>(entity));
        NodeComponent& childNodeComponent = registry.get<NodeComponent>(entity);
        if (childNodeComponent.Parent == entt::null) return;

        childNodeComponent.Parent = entt::null;

        NodeComponent& parentNodeComponent = registry.get<NodeComponent>(childNodeComponent.Parent);

        using It = std::vector<entt::entity>::iterator;
        It it = parentNodeComponent.Children.begin();

        for (; it != parentNodeComponent.Children.end(); ++it)
        {
            if (*it == entity) break;
        }

        if (it == parentNodeComponent.Children.end()) return;
       
        parentNodeComponent.Children.erase(it);

    }

    void VGUtils::AttachTo(entt::registry& registry, entt::entity childEntity, entt::entity parentEntity)
    {
        assert(registry.all_of<NodeComponent>(childEntity) && registry.all_of<NodeComponent>(parentEntity));

        NodeComponent& childNode = registry.get<NodeComponent>(childEntity);
        NodeComponent& parentNode = registry.get<NodeComponent>(parentEntity);

        assert(childNode.Parent == entt::null);

        childNode.Parent = parentEntity;
        parentNode.Children.push_back(childEntity);
    }

}