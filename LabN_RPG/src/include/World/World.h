#pragma once 
#include "SFML/Graphics.hpp"
namespace vg 
{
	class World 
	{
	public:
		World() {};
		virtual ~World() {};

		virtual void Tick(sf::Time deltaTime) {};
		virtual void Render() {};
	};
}