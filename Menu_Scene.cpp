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

  int xTextPos = 20;
  for(auto e : m_menuItems)
  {
    auto text = sf::Text(e, m_font);
    text.setPosition(10, xTextPos);
    text.setFillColor(sf::Color(180, 180, 180));
    xTextPos += 60;
    if(m_menuItems[m_selectedItem] == e)
    {
      text.setFillColor(sf::Color(255, 255, 255));
      text.setOutlineThickness(2);
      text.setOutlineColor(sf::Color(0, 0, 255));
    }
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
			quit();
		}

    if(event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
        case sf::Keyboard::Down:
          m_selectedItem++;
          if(m_selectedItem >= m_menuItems.size())
          {
            m_selectedItem = 0;
          }
          break;
        case sf::Keyboard::Up:
          m_selectedItem--;
          if(m_selectedItem < 0)
          {
            m_selectedItem = m_menuItems.size() - 1;
          }
          break;
        case sf::Keyboard::Return:
          if(m_menuItems[m_selectedItem] == "EXIT")
          {
            quit();
          }
          break;
        default:
          break;
      }
    }
  }
}

void Menu_Scene::quit()
{
  m_game->quit();
}
