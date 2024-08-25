#include "Play_Scene.hpp"
#include "Menu_Scene.hpp"


Play_Scene::Play_Scene(GameEngine* game)
  : Scene(game)
{
  init();
}

void Play_Scene::init()
{
  loadTextures();

  spawnPlayer();
  // spawnBall();
  // spawnRoof();
}

void Play_Scene::update() {
    m_entityManager.update();

    sUserInput();
    sRender();
    if (!m_pause) {
      sCollision();
      sMovement();
      // sScore()
      m_currentFrame++;
    }
}

void Play_Scene::spawnPlayer()
{
  float width = m_game->window().getSize().x / 3; 
  float height = 32;
  auto entity = m_entityManager.addEntity("player");

  entity->cInput = std::make_shared<CInput>();
  entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(width, height));
  entity->cTransform = std::make_shared<CTransform>(
    Vec2(((m_game->window().getSize().x / 2)), m_game->window().getSize().y - entity->cBoundingBox->halfSize.y),
    Vec2(1.0f, 1.0f),
    Vec2(1.0f, 1.0f),
    0.0f
  );
  
  m_player_texture.setRepeated(true);
  entity->sprite = std::make_shared<CSprite>(m_player_texture);
  entity->sprite->sprite.setTextureRect(sf::IntRect(0, 0, width, height));
  entity->sprite->sprite.setOrigin(width/2 , height/2);

  m_player = entity;
}

void Play_Scene::loadTextures()
{
  if (!m_player_texture.loadFromFile("assets/mario_block.png"))
  {
    std::cerr << "Could not load player texture\n";
    exit(-1);
  }

  if (!m_roof_texture.loadFromFile("assets/mario_brick.png"))
  {
    std::cerr << "Could not load roof texture\n";
    exit(-1);
  }
}

void Play_Scene::sMovement()
{
  m_player->cTransform->velocity = {0, 0};

  if (m_player->cInput->left && m_player->cTransform->pos.x > (m_player_speed + m_player->cBoundingBox->halfSize.x))
  {
    m_player->cTransform->velocity.x = -m_player_speed;
  }
  if (m_player->cInput->right && (m_player->cTransform->pos.x + m_player->cBoundingBox->halfSize.x) < m_game->window().getSize().x)
  {
    m_player->cTransform->velocity.x = m_player_speed;
  }

  for (auto e : m_entityManager.getEntities())
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

void Play_Scene::sCollision()
{

  for (auto ball : m_entityManager.getEntities("ball"))
  {
    if((ball->cTransform->pos.x - ball->cBoundingBox->halfSize.x) < 0 || (ball->cTransform->pos.x + ball->cBoundingBox->halfSize.x) > m_game->window().getSize().x)
    {
      ball->cTransform->velocity.x *= -1;
    }

    for (auto player : m_entityManager.getEntities("player"))
    {
      Vec2 overlap = ball->overlap(*player);
      
      if(overlap.y > 0 && overlap.x > 0)
      {
        ball->cTransform->pos.y -= overlap.y;
        ball->cTransform->velocity.y *= -1;
                 
      }
    }

    for (auto top_bar : m_entityManager.getEntities("top_bar"))
    {
      Vec2 overlap = ball->overlap(*top_bar);
      if(overlap.y > 0 && overlap.x > 0)
      {
        ball->cTransform->pos.y += overlap.y;
        ball->cTransform->velocity.y *= -1;
        m_score += 25;
        // ball->rectShape->rectangle.setFillColor(sf::Color(m_score, 0, 0)); 
      }
    }
  }
}

void Play_Scene::sRender()
{
  m_game->window().clear();

  int xTextPos = 20;
  for(auto e : m_entityManager.getEntities())
  {
     float xPos, yPos;
    xPos = e->cTransform->pos.x + e->cTransform->velocity.x;
    yPos = e->cTransform->pos.y + e->cTransform->velocity.y;


    if(e->sprite)
    {
      e->sprite->sprite.setPosition(xPos, yPos);
      m_game->window().draw(e->sprite->sprite);
    }
  }
 
  m_game->window().display();
}

void Play_Scene::sUserInput()
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
        case sf::Keyboard::D:
          m_player->cInput->right = true;
          break;
        case sf::Keyboard::A:
          m_player->cInput->left = true;
          break;
        // case sf::Keyboard::P:
        //   m_game->setPaused();
        //   break;
        case sf::Keyboard::Return:
          m_game->changeScene("MENU", std::make_shared<Menu_Scene>(m_game));
          break;
        default:
          break;
      }
    }
  }
}

void Play_Scene::quit()
{
  m_game->quit();
}