#pragma once 
#include "SFML/Graphics.hpp"
#include "ResourceManagement/TextureProvider.h"
#include "entt/entt.hpp"
#include "Components/Components.h"
#include <fstream>
#include <optional>
#include <utility>
#include "json.hpp"
#include <string>
namespace vg
{
	struct MapLoadingData 
	{
		const std::string LoadingPath;
		const std::string MapFileName;
	};

	class MapFactory
	{
	public:
		MapFactory(TextureProvider* texProvider) :
			m_textureProvider(texProvider)
		{}
		void LoadMap(entt::registry& registry, const MapLoadingData& data);
	private:
		TextureProvider* m_textureProvider;

		void CreateTilemap(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode);
		void ProcessSpawnPlaceholders(entt::registry& registry, nlohmann::json& layerNode);
		void CreateTilesAsIndividuals(entt::registry& registry, nlohmann::json& rootNode, nlohmann::json& layerNode);
		void ProcessProperties(entt::registry& registry, entt::entity entity, nlohmann::json& propertiesNode);
	};
}