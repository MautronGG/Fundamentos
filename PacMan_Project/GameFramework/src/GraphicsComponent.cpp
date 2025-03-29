#include "GraphicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

GraphicsComponent::GraphicsComponent(const sf::Texture& texture)
  : m_sprite(texture) {
}

void GraphicsComponent::Update(const Entity* parent/*DeltaTime*/)
{
  if (parent)
  {
    std::weak_ptr<Transform> transformWeak = parent->GetComponent<Transform>();
    if (auto transform = transformWeak.lock())
    {
      sf::Vector2f newPosition(transform->position.x, transform->position.y);
      m_sprite.setPosition(newPosition);
    }
  }
}