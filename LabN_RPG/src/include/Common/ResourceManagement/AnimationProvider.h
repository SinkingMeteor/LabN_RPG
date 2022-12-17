#pragma once
#include <memory>
#include <string>
#include "SFML/Graphics.hpp"
#include "entt/resource/fwd.hpp"
#include "yaml-cpp/yaml.h"

namespace vg 
{
	struct Animation
	{
		sf::Texture* texture;
		std::vector<sf::IntRect> frames;
	};

	struct AnimationLoadingData
	{
		const std::string& Path;
		sf::Texture* Texture;
	};

	//const std::string& path, sf::Texture* texture
	struct AnimationLoader
	{
		using result_type = std::shared_ptr<Animation>;

		result_type operator()(const AnimationLoadingData& loadingData) const
		{
			std::vector<sf::IntRect> frames{};

			YAML::Node node = YAML::LoadFile(loadingData.Path);

			if (node.IsNull()) return nullptr;

			YAML::Node framesNode = node["animations"][0];

			for (std::size_t i = 0; i < framesNode.size(); ++i)
			{
				YAML::Node rect = framesNode[i];
				if (!rect.IsSequence() || rect.size() != 4) 
				{
					std::cout << "Animation config at path: " << loadingData.Path << "is invalid";
					return nullptr;
				}
				frames.emplace_back(rect[0].as<int>(), rect[1].as<int>(), rect[2].as<int>(), rect[3].as<int>());
			}

			Animation animation{loadingData.Texture, std::move(frames) };
			return std::make_shared<Animation>(std::move(animation));
		}
	};
	using AnimationResource = entt::resource<Animation>;
	using AnimationProvider = entt::resource_cache<Animation, AnimationLoader>;
}