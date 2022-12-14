#pragma once 
#include "SFML/Graphics.hpp"
namespace vg 
{
	class World 
	{
	public:
		virtual ~World() {};
		virtual void Initialize() = 0;
		virtual void Tick(sf::Time deltaTime) = 0;
		virtual void Render() = 0;
	};
}