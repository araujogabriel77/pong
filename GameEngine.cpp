#include "GameEngine.hpp"
#include "Menu_Scene.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

const int PLAYER_SPEED = 10;

GameEngine::GameEngine(const std::string& config)
{
  init();
}

void GameEngine::init()
{
  loadConfiguration();
  changeScene("MENU", std::make_shared<Menu_Scene>(this));
}

void GameEngine::loadConfiguration()
{
  std::cout << "Loading configuration...\n";
  if (!m_font.loadFromFile("fonts/roboto.ttf"))
  {
    std::cerr << "Could not load font\n";
    exit(-1);
  }
  m_text.setCharacterSize(24);
  std::cout << "Font loaded!\n";

  m_window.create(sf::VideoMode(1280, 720), "Pong");
  m_window.setFramerateLimit(60);
  m_window.setVerticalSyncEnabled(true);
}


void GameEngine::run()
{
  while (m_running)
  {
    m_currentScene->update();
  }
}

sf::RenderWindow& GameEngine::window()
{
  return m_window;
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene) {
  m_currentScene = scene;
  m_scenes[sceneName] = scene;
}

void GameEngine::quit()
{
  m_running = false;
  m_window.close();
}