#include "AssetsManager.hpp"

AssetsManager::AssetsManager(const std::string& name, const std::string& filename)
{
  loadTextures(name, filename);
}

void AssetsManager::loadTextures(const std::string& name, const std::string& filename)
{
  sf::Texture texture;
  texture.loadFromFile(filename);
  m_textureMap[name] = texture;
}

sf::Texture& AssetsManager::getTexture(const std::string& name)
{
  return m_textureMap[name];
}