#pragma once

#include "SFML/Graphics.hpp"
#include <map>
#include<memory>
#include<string>

typedef std::map<std::string, sf::Texture> TextureMap;

class AssetsManager
{
  TextureMap m_textureMap;

  public:
  AssetsManager();
};