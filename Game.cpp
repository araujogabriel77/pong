#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

const int PLAYER_SPEED = 15;

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	std::ifstream fin(path);
  std::string configType;
  int window_width, window_height, frameLimit, isFullScreen;

  std::cout << "Loading configuration...\n";
  while (fin >> configType)
  {
    if (configType == "Window")
    {
      fin >> window_width >> window_height >> frameLimit >> isFullScreen;
    }

    if (configType == "Font")
    {
      std::string type, fontFile;
      int size, r, g, b;
      fin >> fontFile >> size >> r >> g >> b;

      if (!m_font.loadFromFile(fontFile))
      {
        std::cerr << "Could not load font\n";
        exit(-1);
      }
      m_text.setCharacterSize(size);
      std::cout << "Font loaded!\n";
    }
  }

  // set up default window parameters
  m_window.create(sf::VideoMode(window_width, window_height), "Pong");
  m_window.setFramerateLimit(frameLimit);
  m_window.setVerticalSyncEnabled(true);

  spawnPlayer();
  spawnTopBar();
}

void Game::run()
{
  while (m_running)
  {
    m_entities.update();
    sUserInput();
    sRender();

    if (!m_paused)
    {
      sCollision();
      sMovement();
      sScore();
      m_currentFrame++;
    }
  }
}

void Game::setPaused()
{
  m_paused = !m_paused;
}

void Game::sScore()
{
  auto scoreText = "Score: " + std::to_string(m_score);
  m_text = sf::Text(scoreText, m_font);
  m_text.setPosition(10, 10);
}

void Game::spawnPlayer()
{
	// auto entity = m_entities.addEntity("player");
	auto entity = m_entities.addEntity("player");

  entity->rectShape = std::make_shared<CRectShape>(
      sf::Vector2f(m_window.getSize().x / 3, 32),
      sf::Color(0, 0, 0),
      sf::Color(255, 0, 0),
      4.0f
  );

  entity->cInput = std::make_shared<CInput>();
  entity->cTransform = std::make_shared<CTransform>(
    Vec2(m_window.getSize().x / 2, m_window.getSize().y - (entity->rectShape->m_size.y / 2)),
    Vec2(1.0f, 1.0f),
    0.0f
    );
  m_player = entity;
}

void Game::spawnTopBar()
{
	auto entity = m_entities.addEntity("top_bar");

  entity->rectShape = std::make_shared<CRectShape>(
      sf::Vector2f(m_window.getSize().x, 16),
      sf::Color(255, 255, 255),
      sf::Color(0, 255, 255),
      4.0f
  );


  entity->cInput = std::make_shared<CInput>();
  entity->cTransform = std::make_shared<CTransform>(
    Vec2((m_window.getSize().x - (entity->rectShape->rectangle.getOutlineThickness() * 2)) / 2, entity->rectShape->m_size.y / 2),
    Vec2(0.0f, 0.0f),
    0.0f
  );
}

void Game::spawnBall(std::shared_ptr<Entity> entity, const Vec2& target)
{
  auto bullet = m_entities.addEntity("ball");

  bullet->circleShape = std::make_shared<CShape>(
      8,
      8,
      sf::Color(255, 0, 0),
      sf::Color(255, 255, 255),
      1);

  Vec2 difference{target.x - entity->cTransform->pos.x, target.y - entity->cTransform->pos.y};
  // Vec2 velocity{m_bulletConfig.S * difference.x, m_bulletConfig.S * difference.y};

  // bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, {1, 1}, 0);
  bullet->cCollision = std::make_shared<CCollision>(8);
}

void Game::sMovement()
{
  m_player->cTransform->velocity = {0, 0};

  // std::cout << "posição do jogador: " << m_player->cTransform->pos.x << std::endl;
  // std::cout << "tamanho do jogador: " << m_player->rectShape->m_size.x << std::endl;
  // std::cout << "posição do jogador - tamanho do jogador: " << m_player->cTransform->pos.x - m_player->rectShape->m_size.x << std::endl;
  // std::cout << m_window.getSize().x << std::endl;
  if (m_player->cInput->left && (m_player->cTransform->pos.x) > PLAYER_SPEED + m_player->rectShape->m_size.x / 2)
  {
    m_player->cTransform->velocity.x = -PLAYER_SPEED;
  }
  if (m_player->cInput->right && (m_player->cTransform->pos.x + (m_player->rectShape->m_size.x / 2)) < m_window.getSize().x - PLAYER_SPEED)
  {
    m_player->cTransform->velocity.x = PLAYER_SPEED;
  }

  for (auto e : m_entities.getEntities())
  {
    if (e->tag() == "player")
    {
      m_player->cTransform->pos += m_player->cTransform->velocity;
    }
    else if (e->cTransform)
    {
      e->cTransform->pos += e->cTransform->velocity;
    }
  }
}

void Game::sCollision()
{
  for (auto b : m_entities.getEntities("ball"))
  {
    for (auto e : m_entities.getEntities("enemy"))
    {
      float dist;
      dist = b->cTransform->pos.dist(e->cTransform->pos);
    }
  }
}

void Game::sRender()
{
  m_window.clear();
  m_player->rectShape->rectangle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

  // draw the entity's sf::CircleShape
  m_window.draw(m_player->rectShape->rectangle);
  m_window.draw(m_text);
  for (auto e : m_entities.getEntities())
  {
    std::cout << e->tag() << std::endl;
    float xPos, yPos;
    xPos = e->cTransform->pos.x + e->cTransform->velocity.x;
    yPos = e->cTransform->pos.y + e->cTransform->velocity.y;

    e->rectShape->rectangle.setPosition(xPos, yPos);

    m_window.draw(e->rectShape->rectangle);
  }
  m_window.display();
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event triggers when te window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			// case sf::Keyboard::W:
			// 	m_player->cInput->up = true;
      //   break;
      // case sf::Keyboard::S:
      //   m_player->cInput->down = true;
      //   break;
      case sf::Keyboard::D:
        m_player->cInput->right = true;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = true;
        break;
      case sf::Keyboard::P:
        setPaused();
        break;
      default:
        break;
      }
    }

    // this event is triggered when a key is released
    if (event.type == sf::Event::KeyReleased)
    {
      switch (event.key.code)
      {
      case sf::Keyboard::W:
        m_player->cInput->up = false;
        break;
      case sf::Keyboard::S:
        m_player->cInput->down = false;
        break;
      case sf::Keyboard::D:
        m_player->cInput->right = false;
        break;
      case sf::Keyboard::A:
        m_player->cInput->left = false;
        break;
      default:
        break;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
      }
    }
  }
}