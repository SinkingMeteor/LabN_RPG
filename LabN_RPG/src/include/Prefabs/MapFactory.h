#pragma once 
#include "SFML/Graphics.hpp"
#include "ResourceManagement/TextureProvider.h"
#include "entt/entt.hpp"
#include "Components/Components.h"
#include <fstream>
#include <optional>
#include <utility>
#include "World/World.h"
#include "json.hpp"
#include <string>
namespace vg
{
	struct MapLoadingData 
	{
	public:
		const std::string LoadingPath;
		const std::string MapFileName;
	};

	struct LayerProperties 
	{
	public:
		bool IsIndividualTiles{false};
		entt::id_type SortingLayerId;
		entt::resource<SlicedTexture> TilemapTexture{};
	};

	class MapFactory
	{
	public:
		MapFactory(TextureProvider* texProvider) :
			m_textureProvider(texProvider)
		{}
		void LoadMap(World* world, entt::registry& registry, const MapLoadingData& data, entt::entity parent);
	private:
		TextureProvider* m_textureProvider;

		void ProcessSpawnPlaceholders(entt::registry& registry, nlohmann::json& layerNode);
		void CreateTiles(World* world, entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode, entt::entity parent);
		void ProcessProperties(nlohmann::json& propertiesNode, LayerProperties& result);
		void SetSortingLayer(entt::registry& registry, entt::entity entity, entt::id_type layerId);
	};
}