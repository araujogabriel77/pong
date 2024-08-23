#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "AssetsManager.hpp"

#include <SFML/Graphics.hpp>

class Game
{
	sf::RenderWindow m_window; // the window we will draw to
	EntityManager m_entities; // vector of entities to maintain
  std::shared_ptr<AssetsManager> m_assetsManager;
	sf::Font m_font; // the font we will use to draw
	sf::Text m_text; 
  int m_score = 0; // the score text to be drawn to the screen
	int m_currentFrame = 0;
	bool m_paused = false; // whether we update game logic
	bool m_running = true; // whether the game is running

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config); // initialize the GameState with a config file path
  void setPaused();                     // pause the game

  void sMovement();  // System: Entity position / movement update
  void sUserInput(); // System: User Input
	void sRender(); // System: Render / Drawing
	void sCollision(); // System: Collisions
	void sScore(); // System: Score

	void spawnPlayer();
	void spawnTopBar();
	void spawnEnemy();
	void spawnBall();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& config); // constructor, takes in game config

	void run();
};

