#include "Prefabs/MapFactory.h"
#include "GameplayUtils.h"
#include "Database.h"
namespace vg 
{
	void MapFactory::LoadMap(entt::registry& registry, const MapLoadingData& data, entt::entity parent)
	{
		assert(registry.all_of<NodeComponent>(parent));

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

			assert(!layer["class"].is_null());
			std::string layerClass = layer["class"].get<std::string>();

			if (layerClass == "Tilemap")
			{
				CreateTiles(registry, rootNode, layer, parent);
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

	 void MapFactory::CreateTiles(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode, entt::entity parent)
	 {
		 std::vector<int> indices = layerNode["data"].get<std::vector<int>>();

		 std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		 std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		 std::size_t tileHeight = rootNode["tileheight"].get<std::size_t>();
		 std::size_t tileWidth = rootNode["tilewidth"].get<std::size_t>();
		 std::size_t vertexCount = mapHeight * mapWidth * 4;

		 LayerProperties layerProperties{};

		 ProcessProperties(layerNode["properties"], layerProperties);

		 for (size_t i = 0; i < indices.size(); ++i)
		 {
			 int& num = indices[i];
			 num -= 1;
			 if (num < 0) continue;

			 entt::entity tileEntity = registry.create();
			 sf::VertexArray vertices{ sf::Quads, 4 };

			 SetSortingLayer(registry, tileEntity, layerProperties.SortingLayerId);

			 TransformComponent& transformComponent = registry.emplace<TransformComponent>(tileEntity);
			 registry.emplace<DirtyTransform>(tileEntity);

			 TextureRect& spriteRect = layerProperties.TilemapTexture->RectDatas[num];

			 TransformComponent& parentTransformComponent = registry.get<TransformComponent>(parent);
			 registry.get<NodeComponent>(parent).Children.push_back(tileEntity);
			 NodeComponent& nodeComponent = registry.emplace<NodeComponent>(tileEntity);
			 nodeComponent.Parent = parent;

			 std::size_t x = i % mapWidth;
			 std::size_t y = i / mapWidth;
			 std::size_t tu = i % (layerProperties.TilemapTexture->Texture.getSize().x / tileWidth);
			 std::size_t tv = i / (layerProperties.TilemapTexture->Texture.getSize().x / tileHeight);

			 const sf::Vector2f offset = sf::Vector2f{ (float)x * spriteRect.Rect.width, (float)y * spriteRect.Rect.width };
			 transformComponent.GlobalTransform.translate(offset);
			 transformComponent.LocalTransform.translate(parentTransformComponent.GlobalTransform.getInverse() * offset);

			 CommonUtils::SetInitialPositionAndTexCoords(vertices, spriteRect, transformComponent);

			 DrawableComponent& tileComponent = registry.emplace<DrawableComponent>(tileEntity);
			 tileComponent.VertexArray = std::move(vertices);
			 tileComponent.RelatedTexture = layerProperties.TilemapTexture.handle().get();
			 tileComponent.RectIndex = num;
		 }
	 }

	 void MapFactory::ProcessProperties(nlohmann::json& propertiesNode, LayerProperties& result)
	 {
		 if (propertiesNode.is_null() || !propertiesNode.is_array()) return;

		 for (nlohmann::json& property : propertiesNode)
		 {
			 entt::id_type propertyId = CommonUtils::StringToId(property["name"]);

			 switch (propertyId)
			 {
			 case Database::LayerProperties::IndividualsTiles:
				 result.IsIndividualTiles = property["value"].get<bool>();
				 break;
			 case Database::LayerProperties::SortingLayer:
				 result.SortingLayerId = CommonUtils::StringToId(property["value"]);
				 break;
			 case Database::LayerProperties::TileMap:
				 result.TilemapTexture = (*m_textureProvider)[CommonUtils::StringToId(property["value"])];
				 break;
			 default:
				 break;
			 }
		 }
	 }
	 void MapFactory::SetSortingLayer(entt::registry& registry, entt::entity entity, entt::id_type layerId)
	 {
		 switch (layerId)
		 {
		 case Database::SortingLayers::Ground:
			 registry.emplace<GroundSortingLayer>(entity);
			 break;
		 case Database::SortingLayers::OnGround:
			 registry.emplace<OnGroundSortingLayer>(entity);
			 break;
		 case Database::SortingLayers::AboveGround:
			 registry.emplace<AboveGroundSortingLayer>(entity);
			 break;
		 default:
			 break;
		 }
	 }
	 //void MapFactory::CreateTilemap(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode, entt::entity parent)
	 //{
		// entt::entity mapEntity = registry.create();
		// DrawableComponent& mapComponent = registry.emplace<DrawableComponent>(mapEntity);
		// TransformComponent& transformComponent = registry.emplace<TransformComponent>(mapEntity);

		// TransformComponent& parentTransformComponent = registry.get<TransformComponent>(parent);
		// registry.get<NodeComponent>(parent).Children.push_back(mapEntity);
		// NodeComponent& nodeComponent = registry.emplace<NodeComponent>(mapEntity);
		// nodeComponent.Parent = parent;

		// transformComponent.LocalTransform.translate(parentTransformComponent.GlobalTransform.getInverse() * VGMath::Zero);

		// entt::resource<SlicedTexture> texture{};

		//ProcessProperties(registry, mapEntity, layerNode["properties"], texture);

		// std::vector<int> indices = layerNode["data"].get<std::vector<int>>();

		// for (int& num : indices) 
		// {
		//	 num -= 1;
		// }

		// std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		// std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		// std::size_t tileHeight = rootNode["tileheight"].get<std::size_t>();
		// std::size_t tileWidth = rootNode["tilewidth"].get<std::size_t>();
		// std::size_t vertexCount = mapHeight * mapWidth * 4;
		// sf::VertexArray vertices{ sf::Quads, vertexCount };

		// for (size_t x = 0; x < mapWidth; ++x)
		// {
		//	 for (size_t y = 0; y < mapHeight; ++y)
		//	 {
		//		 sf::Vertex* quad = &vertices[(x + y * mapWidth) * 4];

		//		 quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
		//		 quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
		//		 quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
		//		 quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

		//		 int tileNumber = indices[x + y * mapWidth];

		//		 if (tileNumber < 0) continue;

		//		 std::size_t tu = tileNumber % (texture->Texture.getSize().x / tileWidth);
		//		 std::size_t tv = tileNumber / (texture->Texture.getSize().x / tileHeight);

		//		 quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
		//		 quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
		//		 quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
		//		 quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
		//	 }
		// }

		// mapComponent.RectsIndices = std::move(indices);
		// mapComponent.VertexArray = std::move(vertices);
		// mapComponent.RelatedTexture = &*texture;
		// mapComponent.SpriteWidthByTiles = mapWidth;
		// mapComponent.SpriteHeightByTiles = mapHeight;
	 //}
}