#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "TransformComponent.h"
struct Vector3
{
  int type = 0;
  int index = 0;
  float x = 0.0f;
  float y = 0.0f;
};

class Tile : public Entity
{
public:
  Tile(int type, float x, float y, int tileSize, const sf::Vector2f& offset, const int index);

  void Render(sf::RenderWindow& window);

  inline const sf::RectangleShape& GetShape() const
  {
    return m_shape;
  }

  inline const sf::FloatRect& GetBounds() const
  {
    return GetShape().getGlobalBounds();
  }
  inline const Vector3 GetInfo() const
  {
    return m_tileInfo;
  }

private:
  sf::RectangleShape m_shape;
  Vector3 m_tileInfo;
};