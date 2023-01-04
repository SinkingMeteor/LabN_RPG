#include "Prefabs/MapFactory.h"
#include "Database.h"
namespace vg 
{
	void MapFactory::LoadMap(entt::registry& registry, const MapLoadingData& data)
	{
		std::string mapFullPath = (data.LoadingPath + data.MapFileName);
		std::ifstream input{ static_cast<const char*>(mapFullPath.c_str()) };
		nlohmann::json rootNode;
		input >> rootNode;
		input.close();

		if (rootNode.is_null()) return;

		nlohmann::json& tilesetNode = rootNode["tilesets"];
		for (nlohmann::json& tileSet : tilesetNode) 
		{
			entt::id_type tilesetId = entt::hashed_string{ tileSet["name"].get<std::string>().c_str() }.value();
			std::string tilesetFileName = tileSet["image"];
			m_textureProvider->load(tilesetId, (data.LoadingPath + tilesetFileName).c_str());
		}

		nlohmann::json& layersNode = rootNode["layers"];

		if(layersNode.is_null() || !layersNode.is_array()) return;

		for (auto& layer : layersNode)
		{
			if (layer.is_null()) continue;

			std::string layerClass = layer["class"].get<std::string>();

			if (layerClass == "Ground")
			{
				ProcessGround(registry, rootNode, layer);
			}
			else if (layerClass == "Objects")
			{
				ProcessTiles(registry, layer);
			}
			else if (layerClass == "SpawnPlaceholders") 
			{
				ProcessSpawnPlaceholders(registry, layer);
			}
		}
	}
	 void MapFactory::ProcessSpawnPlaceholders(entt::registry& registry, nlohmann::json& layerNode)
	 {
		 entt::entity mapEntity = registry.create();
		 PlaceholdersComponent& placeholdersComponent = registry.emplace<PlaceholdersComponent>(mapEntity);
		 nlohmann::json& objectsData = layerNode["objects"];
		 for (auto& object : objectsData)
		 {
			 if (object["class"] == "SpawnPoint")
			 {
				 float x = object["x"].get<float>();
				 float y = object["y"].get<float>();
				 entt::id_type objectName = entt::hashed_string{ object["name"].get<std::string>().c_str() }.value();
				 placeholdersComponent.SpawnPoints.emplace(std::make_pair(objectName, sf::Vector2f{ x, y }));
			 }
		 }
	 }
	 void MapFactory::ProcessGround(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode)
	 {
		 entt::entity mapEntity = registry.create();
		 MapComponent& mapComponent = registry.emplace<MapComponent>(mapEntity);

		 entt::resource<SlicedTexture> texture = (*m_textureProvider)[Database::Textures::DESERT_GROUND_TILESET];
		 
		 std::vector<unsigned int> indices = layerNode["data"].get<std::vector<unsigned int>>();
		 unsigned int mapHeight = rootNode["height"].get<unsigned int>();
		 unsigned int mapWidth = rootNode["width"].get<unsigned int>();
		 unsigned int tileHeight = rootNode["tileheight"].get<unsigned int>();
		 unsigned int tileWidth = rootNode["tilewidth"].get<unsigned int>();
		 unsigned int vertexCount = mapHeight * mapWidth * 4;
		 sf::VertexArray vertices{ sf::Quads, vertexCount };

		 for (size_t x = 0; x < mapWidth; ++x)
		 {
			 for (size_t y = 0; y < mapHeight; ++y)
			 {
				 unsigned int tileNumber = indices[x + y * mapWidth] - 1;

				 unsigned int tu = tileNumber % (texture->Texture.getSize().x / tileWidth);
				 unsigned int tv = tileNumber / (texture->Texture.getSize().x / tileHeight);

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

		 mapComponent.MapIndices = std::move(indices);
		 mapComponent.VertexArray = std::move(vertices);
		 mapComponent.RelatedTexture = &*texture;
		 mapComponent.MapSize = sf::Vector2<unsigned int>{ mapWidth, mapHeight };
		 mapComponent.TileSize = sf::Vector2<unsigned int>{ tileWidth, tileHeight };
	 }
	 void MapFactory::ProcessTiles(entt::registry& registry, nlohmann::json& layerNode)
	 {
		 entt::resource<SlicedTexture> texture = (*m_textureProvider)[Database::Textures::DESERT_GROUND_TILESET];

		 nlohmann::json& objectsData = layerNode["objects"];
		 for (auto& object : objectsData)
		 {
			entt::entity staticObject = registry.create();
			SpriteComponent& spriteComponent = registry.emplace<SpriteComponent>(staticObject);
			std::size_t rectIndex = object["gid"].get<std::size_t>() - 1;
			const TextureRect& rectData = texture->RectDatas[rectIndex];
			spriteComponent.Sprite.setTexture(texture->Texture);
			spriteComponent.Sprite.setOrigin(rectData.Pivot);
			spriteComponent.Sprite.setTextureRect(rectData.Rect);

			float x = object["x"].get<float>();
			float y = object["y"].get<float>();
			registry.emplace<TransformComponent>(staticObject, sf::Vector2f{ x, y }, sf::Vector2f{ 1.0f, 1.0f }, 0.0f);
		 }
	 }
}