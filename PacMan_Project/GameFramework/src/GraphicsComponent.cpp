#include "GraphicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

GraphicsComponent::GraphicsComponent(const sf::Texture& texture)
  : m_sprite(texture) 
{
  m_sprite.setOrigin(m_sprite.getGlobalBounds().getCenter());
}

void GraphicsComponent::Update(const Entity* parent/*DeltaTime*/)
{
  if (parent)
  {
    std::weak_ptr<Transform> transformWeak = parent->GetComponent<Transform>();
    if (auto transform = transformWeak.lock())
    {
      const Vector2& position = parent->GetPosition();
      sf::Vector2f newPosition(transform->position.x, transform->position.y);
      m_sprite.setPosition(newPosition);

      float newRotation(transform->rotation);
      m_sprite.setRotation(sf::degrees(newRotation));
    }
  }
}
void GraphicsComponent::SetTexture(const sf::Texture& texture)
{
  m_sprite.setTexture(texture);
}