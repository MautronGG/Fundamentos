#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class GraphicsComponent : public Component
{
public:
  GraphicsComponent(const sf::Texture& texture);
  ~GraphicsComponent() = default;

  void Update(const Entity* parent/*DeltaTime*/) override;

  inline sf::FloatRect& GetBounds()
  {
    return m_bounds = GetSprite().getGlobalBounds();
  }
  inline const sf::Sprite& GetSprite() const
  {
    return m_sprite;
  }

private:
  sf::Sprite m_sprite;
  sf::FloatRect m_bounds;
};