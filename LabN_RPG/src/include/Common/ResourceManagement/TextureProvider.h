#pragma once
#include <memory>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "entt/resource/fwd.hpp"
#include <cassert>
#include <fstream>
#include "json.hpp"
namespace vg 
{
	struct TextureRect 
	{
	public:
		std::size_t Index;
		sf::IntRect Rect;
		sf::Vector2f Pivot;
	};

	struct SlicedTexture 
	{
	public:
		sf::Texture Texture;
		std::vector<TextureRect> RectDatas;
	};

	struct TextureLoader
	{
		using result_type = std::shared_ptr<SlicedTexture>;

		result_type operator()(const std::string& path) const 
		{
			SlicedTexture slicedTexture{};
			if (!slicedTexture.Texture.loadFromFile(path)) return nullptr;

			std::string metaPath = path + ".json";
			std::ifstream input{};
			input.open(metaPath);
			
			if (input.fail())
			{
				return std::make_shared<SlicedTexture>(std::move(slicedTexture));
			}

			nlohmann::json rootJsonNode{};
			input >> rootJsonNode;
			input.close();

			assert(!rootJsonNode.is_null());

			slicedTexture.RectDatas.reserve(rootJsonNode.size());

			for (nlohmann::json& rectNode : rootJsonNode)
			{
				std::size_t index = rectNode["index"].get<std::size_t>();
				sf::Vector2i rectPosition{};
				rectPosition.x = rectNode["positionX"].get<int>();
				rectPosition.y = rectNode["positionY"].get<int>();
				sf::Vector2i rectSize{};
				rectSize.x = rectNode["sizeX"].get<int>();
				rectSize.y = rectNode["sizeY"].get<int>();
				sf::Vector2f pivotPosition{};
				pivotPosition.x = rectNode["pivotX"].get<float>();
				pivotPosition.y = rectNode["pivotY"].get<float>();

				sf::IntRect rect{ rectPosition, rectSize };

				TextureRect texRect{ index, sf::IntRect{ rectPosition, rectSize }, pivotPosition };
				slicedTexture.RectDatas.emplace_back<TextureRect>(std::move(texRect));
			}

			return std::make_shared<SlicedTexture>(std::move(slicedTexture));
		}
	};

	using TextureResource = entt::resource<SlicedTexture>;
	using TextureProvider = entt::resource_cache<SlicedTexture, TextureLoader>;
}