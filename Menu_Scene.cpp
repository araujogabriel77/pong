#include "Menu_Scene.hpp"

Menu_Scene::Menu_Scene(GameEngine* game)
  : Scene(game)
{
  init();
}

void Menu_Scene::update() {
    m_entityManager.update();

    if (!m_pause) {
        m_currentFrame++;
    }
    sUserInput();
    sRender();
}

void Menu_Scene::init()
{
  m_entityManager = EntityManager();
  if (!m_font.loadFromFile("fonts/roboto.ttf"))
  {
    std::cerr << "Could not load font\n";
    exit(-1);
  }
}

void Menu_Scene::sRender()
{
  m_game->window().clear();

  int xTextPos = 200;
  for(auto e : m_menuItems)
  {
    auto text = sf::Text(e, m_font);
    text.setPosition(10, xTextPos);
    xTextPos += 50;
    m_game->window().draw(text);
  }
 
  m_game->window().display();
}


void Menu_Scene::sUserInput()
{
	sf::Event event;
	while (m_game->window().pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_game->quit();
		}

    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        // m_game->changeScene("N2", std::make_shared<Scene_N2>(m_game));
      }
    }
  }
}