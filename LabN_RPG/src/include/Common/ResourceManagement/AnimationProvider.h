#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "entt/resource/fwd.hpp"
#include "entt/core/hashed_string.hpp"
#include "yaml-cpp/yaml.h"

namespace vg 
{
	struct Animation
	{
		sf::Texture* Texture;
		std::vector<sf::IntRect> Frames;
		float FrameRate;
		entt::id_type AnimationId;
		entt::id_type AnimationGroupId;
	};

	struct AnimationPack
	{
		std::unordered_map<entt::id_type, Animation> Animations{};
	};

	struct AnimationLoadingData
	{
		const std::string& Path;
		sf::Texture* Texture;
	};

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
				entt::id_type id = entt::hashed_string{ animation["name"].as<std::string>().c_str() }.value();
				entt::id_type groupId = entt::hashed_string{ animation["group"].as<std::string>().c_str() }.value();
				float frameRate = animation["frame_rate"].as<float>();
				animationPack.Animations.emplace(id, Animation{loadingData.Texture, std::move(frames), frameRate, id, groupId});
			}
			return std::make_shared<AnimationPack>(std::move(animationPack));
		}
	};
	using AnimationResource = entt::resource<AnimationPack>;
	using AnimationProvider = entt::resource_cache<AnimationPack, AnimationLoader>;
}