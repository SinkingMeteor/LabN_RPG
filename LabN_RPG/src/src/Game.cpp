#include "Game.h"

namespace vg 
{
	Game::Game() :
		m_clock(),
		m_currentWorld(nullptr),
		m_window()
	{}

	void Game::Start()
	{
		m_currentWorld = std::make_unique<GameWorld>(&m_window);
		m_currentWorld->Initialize();
	}

	void Game::Update()
	{
		while (m_window.Update())
		{
			if (!m_currentWorld) return;

			sf::Time passedTime = m_clock.restart();
			sf::Time deltaTime = sf::seconds(1.0f / 60.0f);

			while (passedTime > deltaTime)
			{
				passedTime -= deltaTime;
				Tick(deltaTime);
			}

			Tick(passedTime);
			Render();
		}

	}

	void Game::Tick(sf::Time deltaTime)
	{
		m_currentWorld->Tick(deltaTime);
	}

	void Game::Render()
	{
		m_currentWorld->Render();
	}
}

