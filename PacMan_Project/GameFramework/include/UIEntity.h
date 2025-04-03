#pragma once

#include <string>
#include "Entity.h"

enum class UIType {Title, Button};

class UIEntity : public Entity
{
public:
  void SetShape(UIType type, int fontSize, std::string text, sf::Vector2f position, sf::Text textObject);

  inline const UIType GetType() const
  {
    return m_type;
  }

  UIType m_type;
  sf::RectangleShape m_shape;
  sf::Texture m_texture;
};