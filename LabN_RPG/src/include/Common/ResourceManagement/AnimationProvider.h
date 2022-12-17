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

	struct AnimationPack
	{
		std::unordered_map<std::string, Animation> Animations{};
	};

	struct AnimationLoadingData
	{
		const std::string& Path;
		sf::Texture* Texture;
	};

	//const std::string& path, sf::Texture* texture
	struct AnimationLoader
	{
		using result_type = std::shared_ptr<AnimationPack>;

		result_type operator()(const AnimationLoadingData& loadingData) const
		{
			AnimationPack animationPack{};

			YAML::Node node = YAML::LoadFile(loadingData.Path);

			if (node.IsNull()) return nullptr;

			YAML::Node animations = node["animations"];

			for (auto animation : animations)
			{
				std::vector<sf::IntRect> frames{};
				YAML::Node framesNode = animation["frames"];
				for (auto frame : framesNode)
				{
					if (!frame.IsSequence() || frame.size() != 4)
					{
						std::cout << "Animation config at path: " << loadingData.Path << "is invalid";
						return nullptr;
					}
					frames.emplace_back(frame[0].as<int>(), frame[1].as<int>(), frame[2].as<int>(), frame[3].as<int>());
				}
				animationPack.Animations.emplace(animation["name"].as<std::string>(), Animation{loadingData.Texture, std::move(frames)});
			}
			return std::make_shared<AnimationPack>(std::move(animationPack));
		}
	};
	using AnimationResource = entt::resource<AnimationPack>;
	using AnimationProvider = entt::resource_cache<AnimationPack, AnimationLoader>;
}