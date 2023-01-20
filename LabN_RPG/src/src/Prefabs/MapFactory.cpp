#include "Prefabs/MapFactory.h"
#include "GameplayUtils.h"
#include "Database.h"
namespace vg 
{
	void MapFactory::LoadMap(World* world, entt::registry& registry, const MapLoadingData& data, entt::entity parent)
	{
		assert(registry.all_of<NodeComponent>(parent));

		std::string mapFullPath = (data.LoadingPath + data.MapFileName);
		std::ifstream input{ static_cast<const char*>(mapFullPath.c_str()) };
		nlohmann::json rootNode;
		input >> rootNode;
		input.close();

		assert(world);
		assert(!rootNode.is_null());

		entt::entity worldRoot = world->GetSceneRootEntity();

		assert(registry.all_of<WorldPartitionComponent>(worldRoot));

		WorldPartitionComponent& worldPartitionComponent = registry.get<WorldPartitionComponent>(worldRoot);
		entt::entity partitionGrid = registry.create();
		std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		float tileSize = rootNode["tilewidth"].get<float>();
		float cellSize = tileSize * 2.0f;
		std::size_t gridWidth = std::ceil((float)(mapWidth * tileSize) / cellSize);
		std::size_t gridHeight = std::ceil((float)(mapHeight * tileSize) / cellSize);
		worldPartitionComponent.Grid = PartitionGrid{ gridWidth, gridHeight, cellSize };
			
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

			entt::id_type layerClass = CommonUtils::StringToId(layer["class"]);

			switch (layerClass) 
			{
			case Database::LayerClass::Tilemap:
				CreateTiles(world, registry, rootNode, layer, parent); break;
			case Database::LayerClass::SpawnPlaceholders:
				ProcessSpawnPlaceholders(registry, layer); break;
			case Database::LayerClass::Colliders:
				ProcessColliders(world, registry, layer, parent); break;
			default:
				break;
			}
		}
	}
	 void MapFactory::ProcessSpawnPlaceholders(entt::registry& registry, nlohmann::json& layerNode)
	 {
		 entt::entity mapEntity = registry.create();
		 PlaceholdersComponent& placeholdersComponent = registry.emplace<PlaceholdersComponent>(mapEntity);
		 nlohmann::json& objectsData = layerNode["objects"];
		 for (nlohmann::json& object : objectsData)
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

	 void MapFactory::ProcessColliders(World* world, entt::registry& registry, nlohmann::json& layerNode, entt::entity parent)
	 {
		 nlohmann::json& collidersData = layerNode["objects"];
		 assert(!collidersData.is_null());

		 WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(world->GetSceneRootEntity());

		 for (nlohmann::json& colliderNode : collidersData)
		 {
			 entt::entity colliderEntity = registry.create();
			 TransformComponent& transformComponent = registry.emplace<TransformComponent>(colliderEntity);
			 RectColliderComponent& colliderComponent = registry.emplace<RectColliderComponent>(colliderEntity);

			 NodeComponent& nodeComponent = registry.emplace<NodeComponent>(colliderEntity);
			 nodeComponent.Parent = parent;
			 registry.get<NodeComponent>(parent).Children.push_back(colliderEntity);

			 float x = colliderNode["x"].get<float>();
			 float y = colliderNode["y"].get<float>();
			 float width = colliderNode["width"].get<float>();
			 float height = colliderNode["height"].get<float>();

			 sf::Vector2f globalPosition{ x, y };
			 sf::Vector2f size{ width, height };

			 transformComponent.GlobalTransform.translate(globalPosition);
			 transformComponent.LocalTransform.translate(registry.get<TransformComponent>(parent).GlobalTransform.getInverse() * globalPosition);

			 colliderComponent.Rect = sf::FloatRect{ sf::Vector2f{}, size };

			 sf::FloatRect colliderRect = sf::FloatRect{ globalPosition, size };
			 std::vector<std::size_t> cellIndices{};
			 partitionGrid.Grid.GetAllCellsContainsRect(colliderRect, cellIndices);

			 for (size_t i = 0; i < cellIndices.size(); ++i)
			 {
				 partitionGrid.Grid.GetCell(cellIndices[i]).AddEntity(colliderEntity);
			 }
		 }
	 }

	 void MapFactory::CreateTiles(World* world, entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode, entt::entity parent)
	 {
		 std::vector<int> indices = layerNode["data"].get<std::vector<int>>();
		 std::size_t mapHeight = rootNode["height"].get<std::size_t>();
		 std::size_t mapWidth = rootNode["width"].get<std::size_t>();
		 std::size_t tileHeight = rootNode["tileheight"].get<std::size_t>();
		 std::size_t tileWidth = rootNode["tilewidth"].get<std::size_t>();
		 std::size_t vertexCount = mapHeight * mapWidth * 4;

		 WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(world->GetSceneRootEntity());

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

			 TextureRect& spriteRect = layerProperties.TilemapTexture->RectDatas[num];

			 TransformComponent& parentTransformComponent = registry.get<TransformComponent>(parent);
			 registry.get<NodeComponent>(parent).Children.push_back(tileEntity);
			 NodeComponent& nodeComponent = registry.emplace<NodeComponent>(tileEntity);
			 nodeComponent.Parent = parent;

			 std::size_t x = i % mapWidth;
			 std::size_t y = i / mapWidth;
			 std::size_t tu = i % (layerProperties.TilemapTexture->Texture.getSize().x / tileWidth);
			 std::size_t tv = i / (layerProperties.TilemapTexture->Texture.getSize().x / tileHeight);

			 //Для соответствия позиций тайлов в редакторе и игре я добавляю пивот. 
			 //Но только к тайлам, к персонажам и всему чего в редакторе нет, добавлять не нужно.
			 const sf::Vector2f offset = sf::Vector2f{ (float)x * spriteRect.Rect.width, (float)y * spriteRect.Rect.width } + spriteRect.Pivot;
			 transformComponent.GlobalTransform.translate(offset);
			 transformComponent.LocalTransform.translate(parentTransformComponent.GlobalTransform.getInverse() * offset);

			 PartitionCell& cell = partitionGrid.Grid.GetCell(transformComponent.GlobalTransform * VGMath::One);
			 cell.AddEntity(tileEntity);

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
}