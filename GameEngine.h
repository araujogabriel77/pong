#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Scene;

class GameEngine
{
  protected:

  std::map<std::string, std::shared_ptr<Scene>> m_scenes;
  std::shared_ptr<Scene> m_currentScene;
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	sf::Text m_text;
	bool m_running = true;
  
	std::shared_ptr<Entity> m_player;

  void sUserInput();
	void sRender();

  void loadConfiguration();
	void spawnPlayer();

  void init();
public:
	GameEngine(const std::string& config);

  void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);
	void run();
  void quit();
  sf::RenderWindow& window();
};

