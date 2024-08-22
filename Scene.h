#pragma once

#include "GameEngine.h"
#include "EntityManager.h"
#include <map>

typedef std::map<int, std::string> ActionMap;

class GameEngine;

class Scene
{
  protected:
  
  GameEngine* m_game = nullptr;
  EntityManager m_entityManager;
  ActionMap m_actionMap;
  bool m_pause = false;
  bool m_hasEnded = false;
  size_t m_currentFrame = 0;

  public:

  Scene();
  Scene(GameEngine *game);
  virtual ~Scene();

  virtual void update() = 0;
  virtual void sRender() = 0;
};