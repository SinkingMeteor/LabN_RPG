#pragma once
#include <memory>
#include <string>
#include "SFML/Graphics.hpp"
#include "entt/resource/fwd.hpp"
namespace vg 
{
	struct TextureLoader
	{
		using result_type = std::shared_ptr<sf::Texture>;

		result_type operator()(const std::string& path) const 
		{
			sf::Texture texture{};
			if (!texture.loadFromFile(path)) return nullptr;

			return std::make_shared<sf::Texture>(std::move(texture));
		}
	};

	using TextureResource = entt::resource<sf::Texture>;
	using TextureProvider = entt::resource_cache<sf::Texture, TextureLoader>;
}