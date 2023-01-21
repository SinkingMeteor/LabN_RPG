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

			entt::id_type layerClass = VGUtils::StringToId(layer["class"]);

			switch (layerClass) 
			{
			case Database::LayerClass::Tilemap:
				CreateTiles(world, registry, rootNode, layer, parent); break;
			case Database::LayerClass::SpawnPlaceholders:
				ProcessSpawnPlaceholders(world, registry, layer); break;
			case Database::LayerClass::Colliders:
				ProcessColliders(world, registry, layer, parent); break;
			default:
				break;
			}
		}
	}
	 void MapFactory::ProcessSpawnPlaceholders(World* world, entt::registry& registry, nlohmann::json& layerNode)
	 {
		 nlohmann::json& objectsData = layerNode["objects"];

		 entt::entity worldRoot = world->GetSceneRootEntity();
		 TransformComponent& rootTransformComponent = registry.get<TransformComponent>(worldRoot);
		 NodeComponent& rootNodeComponent = registry.get<NodeComponent>(worldRoot);
		 WorldPartitionComponent& rootPartitionComponent = registry.get<WorldPartitionComponent>(worldRoot);

		 for (nlohmann::json& object : objectsData)
		 {
			 if (object["class"] == "SpawnPoint")
			 {
				 assert(!object["name"].is_null() && object["name"].is_string());
				 
				 entt::id_type pointId = VGUtils::StringToId(object["name"]);
				 entt::entity placeholderEntity = registry.create();
				 world->AddUniqueObject(pointId, placeholderEntity);
				 float x = object["x"].get<float>();
				 float y = object["y"].get<float>();
				 sf::Vector2f startPosition{ x, y };

				 NodeComponent& nodeComponent = registry.emplace<NodeComponent>(placeholderEntity);
				 VGUtils::AttachTo(registry, placeholderEntity, worldRoot);

				 TransformComponent& transformComponent = registry.emplace<TransformComponent>(placeholderEntity);
				 transformComponent.GlobalTransform.translate(startPosition);
				 transformComponent.LocalTransform.translate(rootTransformComponent.GlobalTransform.getInverse() * startPosition);

				 PartitionCell& cell = rootPartitionComponent.Grid.GetCell(startPosition);
				 cell.AddEntity(placeholderEntity);
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

			 std::string colliderName{ colliderNode["name"] };
			 if (colliderName.size() != 0) 
			 {
				 world->AddUniqueObject(VGUtils::StringToId(colliderName), colliderEntity);
			 }
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
		 int offsetX = 0;
		 int offsetY = 0;

		 if (!layerNode["offsetx"].is_null() && !layerNode["offsety"].is_null()) 
		 {
			 offsetX = layerNode["offsetx"].get<int>();
			 offsetY = layerNode["offsety"].get<int>();
		 }

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
			 const sf::Vector2f offset = sf::Vector2f{ (float)x * spriteRect.Rect.width + offsetX, (float)y * spriteRect.Rect.height + offsetY } + spriteRect.Pivot;
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
			 entt::id_type propertyId = VGUtils::StringToId(property["name"]);

			 switch (propertyId)
			 {
			 case Database::LayerProperties::IndividualsTiles:
				 result.IsIndividualTiles = property["value"].get<bool>();
				 break;
			 case Database::LayerProperties::SortingLayer:
				 result.SortingLayerId = VGUtils::StringToId(property["value"]);
				 break;
			 case Database::LayerProperties::TileMap:
				 result.TilemapTexture = (*m_textureProvider)[VGUtils::StringToId(property["value"])];
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