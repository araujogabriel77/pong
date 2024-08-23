#pragma once

#include "Scene.hpp"
#include <vector>

class Menu_Scene: public Scene
{
  std::vector<std::string> m_menuItems = {"START", "EXIT"};
  int m_selectedItem = 0;
  // TODO: PASSAR PARA O ASSETS MANAGER
  sf::Font m_font;

  void init();
  void sUserInput();
  void sRender();

  public:
  Menu_Scene(GameEngine* game);
  void update();
};