#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

const int PLAYER_SPEED = 10;

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
  m_assetsManager = std::make_shared<AssetsManager>("ball", "assets/ball.png");
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
  spawnBall();
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
  float width = m_window.getSize().x / 3;
  float height = 32;
	auto entity = m_entities.addEntity("player");

  entity->rectShape = std::make_shared<CRectShape>(
      sf::Vector2f(width, height),
      sf::Color(0, 0, 0),
      sf::Color(255, 0, 0),
      4.0f
  );

  entity->cInput = std::make_shared<CInput>();
  entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(width, height));
  entity->cTransform = std::make_shared<CTransform>(
    Vec2(((m_window.getSize().x / 2)), m_window.getSize().y - entity->cBoundingBox->halfSize.y),
    Vec2(((m_window.getSize().x / 2)), m_window.getSize().y - entity->cBoundingBox->halfSize.y),
    Vec2(1.0f, 1.0f),
    Vec2(1.0f, 1.0f),
    0.0f
    );
  m_player = entity;
}

void Game::spawnTopBar()
{ 
  float width = m_window.getSize().x;
  float height = 16;
	auto entity = m_entities.addEntity("top_bar");

  entity->rectShape = std::make_shared<CRectShape>(
      sf::Vector2f(width, height),
      sf::Color(255, 255, 255),
      sf::Color(0, 255, 255),
      4.0f
  );

  entity->cInput = std::make_shared<CInput>();
  entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(width, height));
  entity->cTransform = std::make_shared<CTransform>(
    Vec2(((m_window.getSize().x / 2)), entity->cBoundingBox->halfSize.y),
    Vec2(((m_window.getSize().x / 2)), entity->cBoundingBox->halfSize.y),
    Vec2(1.0f, 1.0f),
    Vec2(0.0f, 0.0f),
    0.0f
  );
}

void Game::spawnBall()
{
  float width = 20;
  float height = 20;
  auto entity = m_entities.addEntity("ball");

  float ex = m_window.getSize().x / 2;
  float ey = m_window.getSize().y / 2;
  float targetX = rand() % m_window.getSize().x;
  float targetY = rand() % m_window.getSize().y;

  Vec2 difference{targetX - ex, targetY - ey};
  difference.normalize();
  Vec2 velocity{8 * difference.x, 8 * difference.y};

  entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(width, height));
  entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(ex, ey), Vec2(1.0f, 1.0f), velocity, 0.0f);
  entity->cInput = std::make_shared<CInput>();
  entity->rectShape = std::make_shared<CRectShape>(
      sf::Vector2f(width, height),
      sf::Color(255, 255, 255),
      sf::Color(255, 0, 255),
      4.0f
  );
  float spriteW = 70;
  float spriteH = 104;
  entity->sprite = std::make_shared<sf::Sprite>(m_assetsManager->getTexture("ball"));
  entity->sprite->setTextureRect(sf::IntRect(0, 0, spriteW, spriteH));
  entity->sprite->setScale(0.5, 0.5);
  entity->sprite->setOrigin(spriteW / 2, spriteH / 2);
}

void Game::sMovement()
{
  m_player->cTransform->velocity = {0, 0};

  if (m_player->cInput->left && m_player->cTransform->pos.x > (PLAYER_SPEED + m_player->cBoundingBox->halfSize.x))
  {
    m_player->cTransform->velocity.x = -PLAYER_SPEED;
  }
  if (m_player->cInput->right && (m_player->cTransform->pos.x + m_player->cBoundingBox->halfSize.x) < m_window.getSize().x)
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

  for (auto ball : m_entities.getEntities("ball"))
  {
    if((ball->cTransform->pos.x - ball->cBoundingBox->halfSize.x) < 0 || (ball->cTransform->pos.x + ball->cBoundingBox->halfSize.x) > m_window.getSize().x)
    {
      ball->cTransform->velocity.x *= -1;
    }

    for (auto player : m_entities.getEntities("player"))
    {
      Vec2 overlap = ball->overlap(*player);
      
      if(overlap.y > 0 && overlap.x > 0)
      {
        ball->cTransform->pos.y -= overlap.y;
        ball->cTransform->velocity.y *= -1;
                 
      }
    }

    for (auto top_bar : m_entities.getEntities("top_bar"))
    {
      Vec2 overlap = ball->overlap(*top_bar);
      if(overlap.y > 0 && overlap.x > 0)
      {
        ball->cTransform->pos.y += overlap.y;
        ball->cTransform->velocity.y *= -1;
        m_score += 25;
        ball->rectShape->rectangle.setFillColor(sf::Color(m_score, 0, 0)); 
      }
    }
  }
}

void Game::sRender()
{
  m_window.clear();
  m_player->rectShape->rectangle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

  m_window.draw(m_player->rectShape->rectangle);
  m_window.draw(m_text);
  for (auto e : m_entities.getEntities())
  {
    float xPos, yPos;
    xPos = e->cTransform->pos.x + e->cTransform->velocity.x;
    yPos = e->cTransform->pos.y + e->cTransform->velocity.y;

    e->rectShape->rectangle.setPosition(xPos, yPos);
    if(e->sprite) {
      e->sprite->setPosition(xPos, yPos);
      m_window.draw(*e->sprite);
    }

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