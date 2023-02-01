#pragma once
#include "vgpch.h"
#include "TextureProvider.h"

namespace vg 
{
	struct Animation
	{
		SlicedTexture* Texture{};
		std::vector<std::size_t> Frames{};
		float FrameRate{};
		entt::id_type AnimationId{};
		entt::id_type AnimationGroupId{};
	};

	struct AnimationPack
	{
		std::unordered_map<entt::id_type, Animation> Animations{};
	};

	struct AnimationLoadingData
	{
		const std::string& Path;
		SlicedTexture* Texture;
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
				std::vector<std::size_t> frames{};
				auto& framesNode = animation["frames"];
				for (auto frame : framesNode)
				{
					frames.emplace_back(frame.get<std::size_t>());
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