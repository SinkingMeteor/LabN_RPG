#pragma once
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "entt/resource/fwd.hpp"
#include "entt/core/hashed_string.hpp"
#include "json.hpp"

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

			std::ifstream input{ static_cast<const char*>(loadingData.Path.c_str()) };
			nlohmann::json jsonNode;
			input >> jsonNode;
			input.close();

			if (jsonNode.is_null()) return nullptr;

			auto& animations = jsonNode["animations"];

			for (auto& animation : animations)
			{
				std::vector<sf::IntRect> frames{};
				auto& framesNode = animation["frames"];
				for (auto frame : framesNode)
				{
					if (!frame.is_array() || frame.size() != 4)
					{
						std::cout << "Animation config at path: " << loadingData.Path << "is invalid";
						return nullptr;
					}
					frames.emplace_back(frame[0].get<int>(), frame[1].get<int>(), frame[2].get<int>(), frame[3].get<int>());
				}
				entt::id_type id = entt::hashed_string{ animation["name"].get<std::string>().c_str() }.value();
				entt::id_type groupId = entt::hashed_string{ animation["group"].get<std::string>().c_str() }.value();
				float frameRate = animation["frame_rate"].get<float>();
				animationPack.Animations.emplace(id, Animation{loadingData.Texture, std::move(frames), frameRate, id, groupId});
			}
			return std::make_shared<AnimationPack>(std::move(animationPack));
		}
	};
	using AnimationResource = entt::resource<AnimationPack>;
	using AnimationProvider = entt::resource_cache<AnimationPack, AnimationLoader>;
}