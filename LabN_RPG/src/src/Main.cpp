#include <iostream>
#include "Window/Window.h"
#include "Locator.h"
#include "Game.h"
int main() 
{
	vg::Locator::Window_t::emplace<vg::Window>();
	vg::Window& window = vg::Locator::Window_t::value();

	vg::Game game{};
	while(window.Update())
	{
		game.Update();
	}
	return 0;
}