#include <iostream>
#include "Window/Window.h"
#include "entt/locator/locator.hpp"
#include "Locator.h"
#include "Game.h"
int main() 
{
	vg::Window& window = vg::Locator::GameWindow::emplace<vg::Window>();
	vg::Game game{};
	while(window.Update())
	{
		game.Update();
	}
	return 0;
}