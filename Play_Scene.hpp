#pragma once

#include "Scene.hpp"
#include <vector>


class Play_Scene: public Scene
{
  int m_player_speed = 10;
  int m_score = 0;
	int m_currentFrame = 0;
	bool m_paused = false;
	bool m_running = true;

	std::shared_ptr<Entity> m_player;
	std::shared_ptr<Entity> m_ball;
	std::shared_ptr<Entity> m_roof;

  sf::Texture m_player_texture;
  sf::Texture m_roof_texture;
  sf::Texture m_ball_texture;

  void init();
  void quit();
  void loadTextures();

  void spawnPlayer();
  void spawnBall();
  void spawnRoof();

  void sUserInput();
  void sRender();
  void sMovement();
  void sCollision();
  void sScore();

  public:
  Play_Scene(GameEngine* game);
  void update();
};