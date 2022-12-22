#include "Prefabs/MapFactory.h"

namespace vg 
{
	 std::optional<entt::entity> MapFactory::CreateEntity(entt::registry& registry, const MapLoadingData& data)
	{
		if(!m_textureProvider->contains(data.RelatedTexture)) return std::optional<entt::entity>{};
		
		entt::resource<sf::Texture> texture = (*m_textureProvider)[data.RelatedTexture];

		if(!texture) return std::optional<entt::entity>{};

		std::ifstream input{ static_cast<const char*>(data.LoadingPath.c_str()) };
		nlohmann::json rootNode;
		input >> rootNode;
		input.close();

		if (rootNode.is_null()) return std::optional<entt::entity>{};

		nlohmann::json layersNode = rootNode["layers"];

		if(layersNode.is_null() || !layersNode.is_array()) return std::optional<entt::entity>{};

		std::vector<unsigned int> indices{};

		for (auto& layer : layersNode)
		{
			indices = layer["data"].get<std::vector<unsigned int>>();
		}
		unsigned int mapHeight = rootNode["height"].get<unsigned int>();
		unsigned int mapWidth = rootNode["width"].get<unsigned int>();
		unsigned int tileHeight = rootNode["tileheight"].get<unsigned int>();
		unsigned int tileWidth = rootNode["tilewidth"].get<unsigned int>();
		unsigned int vertexCount = mapHeight * mapWidth * 4;
		entt::entity mapEntity = registry.create();
		sf::VertexArray vertices{ sf::Quads, vertexCount };
		for (size_t x = 0; x < mapWidth; ++x)
		{
			for (size_t y = 0; y < mapHeight; ++y)
			{
				unsigned int tileNumber = indices[x + y * mapWidth] - 1;

				unsigned int tu = tileNumber % (texture->getSize().x / tileWidth);
				unsigned int tv = tileNumber / (texture->getSize().x / tileHeight);

				sf::Vertex* quad = &vertices[(x + y * mapWidth) * 4];

				quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
				quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
				quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
				quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

				quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
				quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
			}
		}

		MapComponent& mapComponent = registry.emplace<MapComponent>(mapEntity);
		mapComponent.MapIndices = std::move(indices);
		mapComponent.VertexArray = std::move(vertices);
		mapComponent.RelatedTexture = &*texture;
		mapComponent.MapWidth = mapWidth;
		mapComponent.MapHeight = mapHeight;
		mapComponent.TileSizeX = tileWidth;
		mapComponent.TileSizeY = tileHeight;
		return mapEntity;
	}
}