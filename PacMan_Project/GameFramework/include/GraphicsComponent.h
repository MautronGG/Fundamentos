#pragma once

#include "Component.h"
class GraphicsComponent : public Component
{
public:
  GraphicsComponent(const sf::Texture& texture);
  ~GraphicsComponent() = default;

  void SetTexture(const sf::Texture& texture);
  void Update(const Entity* parent/*DeltaTime*/) override;

  inline sf::FloatRect& GetBounds()
  {
    return m_bounds = GetSprite().getGlobalBounds();
  }
  inline const sf::Sprite& GetSprite() const
  {
    return m_sprite;
  }
  void SetSpriteColor(const sf::Color& color)
  {
    m_sprite.setColor(color);
  }

private:
  sf::Sprite m_sprite;
  sf::FloatRect m_bounds;
};