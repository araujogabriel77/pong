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
  AssetsManager(const std::string& name, const std::string& filename);

  void loadTextures(const std::string& name, const std::string& filename);
  sf::Texture& getTexture(const std::string& name);
};