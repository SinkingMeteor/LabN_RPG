#include <iostream>
#include "Window/Window.h"
#include "entt/locator/locator.hpp"
#include "Locator.h"
#include "Game.h"
#include "yaml-cpp/yaml.h"


int main() 
{
	YAML::Node node = YAML::LoadFile("./resources/Animations/Anim_Hero.yaml");
	std::cout << node.Type() << std::endl;
	YAML::Node frames = node["animations"];
	if (frames.Type() == YAML::NodeType::Sequence)
	{
		std::cout << frames.size() << std::endl;
	}


	vg::Window& window = vg::Locator::GameWindow::emplace<vg::Window>();
	vg::Game game{};
	while(window.Update())
	{
		game.Update();
	}
	return 0;
}