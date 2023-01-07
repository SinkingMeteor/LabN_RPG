#include "Prefabs/MapFactory.h"
#include "GameplayUtils.h"
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

			if (layerClass == "Tilemap")
			{
				nlohmann::json& propertiesNode = layer["properties"];
				if (!propertiesNode.is_null() && propertiesNode.is_array())
				{
					for (nlohmann::json& propertyNode : propertiesNode)
					{
						if (!propertyNode.is_null() && propertyNode["name"] == "individualSorting")
						{
							if (propertyNode["value"].get<bool>())
							{
								CreateTilesAsIndividuals(registry, rootNode, layer);
							}
							else
							{
								CreateTilemap(registry, rootNode, layer);
							}
						}
					}
				}
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
	 void MapFactory::CreateTilesAsIndividuals(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode)
	 {
		 entt::resource<SlicedTexture> texture = (*m_textureProvider)[Database::Textures::DESERT_GROUND_TILESET];
		 std::vector<int> indices = layerNode["data"].get<std::vector<int>>();

		 std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		 std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		 std::size_t tileHeight = rootNode["tileheight"].get<std::size_t>();
		 std::size_t tileWidth = rootNode["tilewidth"].get<std::size_t>();
		 std::size_t vertexCount = mapHeight * mapWidth * 4;
		 sf::VertexArray vertices{ sf::Quads, vertexCount };

		 for (size_t i = 0; i < indices.size(); ++i)
		 {
			 int& num = indices[i];
			 num -= 1;
			 if (num < 0) continue;

			 entt::entity tileEntity = registry.create();
			 sf::VertexArray vertices{ sf::Quads, 4 };

			 TransformComponent& transformComponent = registry.emplace<TransformComponent>(tileEntity);
			 TextureRect& spriteRect = texture->RectDatas[num];

			 std::size_t x = i % mapWidth;
			 std::size_t y = i / mapWidth;
			 std::size_t tu = i % (texture->Texture.getSize().x / tileWidth);
			 std::size_t tv = i / (texture->Texture.getSize().x / tileHeight);

			 const sf::Vector2f offset = sf::Vector2f{ (float)x * spriteRect.Rect.width, (float)y * spriteRect.Rect.width };
			 transformComponent.Origin = spriteRect.Pivot;
			 transformComponent.Transform.translate(offset);

			 GameplayUtils::SetInitialPositionAndTexCoords(vertices, spriteRect, transformComponent);

			 ProcessProperties(registry, tileEntity, layerNode["properties"]);

			 DrawableComponent& tileComponent = registry.emplace<DrawableComponent>(tileEntity);
			 tileComponent.RectsIndices.push_back(num);
			 tileComponent.VertexArray = std::move(vertices);
			 tileComponent.RelatedTexture = &*texture;
			 tileComponent.SpriteWidthByTiles = 1;
			 tileComponent.SpriteHeightByTiles = 1;
		 }
	 }

	 void MapFactory::ProcessProperties(entt::registry& registry, entt::entity entity, nlohmann::json& propertiesNode)
	 {
		 if (propertiesNode.is_null()) return;

		 nlohmann::json& sortingLayerNode = propertiesNode[1];
		 if (!sortingLayerNode.is_null() && sortingLayerNode["name"] == "sortingLayer")
		 {
			 entt::id_type layerId = entt::hashed_string{ sortingLayerNode["value"].get<std::string>().c_str() }.value();
			 if (layerId == Database::SortingLayers::Ground)
			 {
				 registry.emplace<GroundSortingLayer>(entity);
			 }
			 else if (layerId == Database::SortingLayers::OnGround)
			 {
				 registry.emplace<OnGroundSortingLayer>(entity);
			 }
		 }
	 }
	 void MapFactory::CreateTilemap(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode)
	 {
		 entt::entity mapEntity = registry.create();
		 DrawableComponent& mapComponent = registry.emplace<DrawableComponent>(mapEntity);
		 registry.emplace<TransformComponent>(mapEntity);

		ProcessProperties(registry, mapEntity, layerNode["properties"]);

		 entt::resource<SlicedTexture> texture = (*m_textureProvider)[Database::Textures::DESERT_GROUND_TILESET];

		 std::vector<int> indices = layerNode["data"].get<std::vector<int>>();

		 for (int& num : indices) 
		 {
			 num -= 1;
		 }

		 std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		 std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		 std::size_t tileHeight = rootNode["tileheight"].get<std::size_t>();
		 std::size_t tileWidth = rootNode["tilewidth"].get<std::size_t>();
		 std::size_t vertexCount = mapHeight * mapWidth * 4;
		 sf::VertexArray vertices{ sf::Quads, vertexCount };

		 for (size_t x = 0; x < mapWidth; ++x)
		 {
			 for (size_t y = 0; y < mapHeight; ++y)
			 {
				 std::size_t tileNumber = indices[x + y * mapWidth];

				 std::size_t tu = tileNumber % (texture->Texture.getSize().x / tileWidth);
				 std::size_t tv = tileNumber / (texture->Texture.getSize().x / tileHeight);

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

		 mapComponent.RectsIndices = std::move(indices);
		 mapComponent.VertexArray = std::move(vertices);
		 mapComponent.RelatedTexture = &*texture;
		 mapComponent.SpriteWidthByTiles = mapWidth;
		 mapComponent.SpriteHeightByTiles = mapHeight;
	 }
}