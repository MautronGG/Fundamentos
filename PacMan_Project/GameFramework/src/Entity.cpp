#include "Component.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Tile.h"
#include "Vector2.h"

Entity::Entity()
{
  m_components.push_back(std::make_shared<Transform>());
}

void Entity::Update(/*DeltaTime*/)
{
  for (const auto& component : m_components)
  {
    component->Update(this/*DeltaTime*/);
  }
}
const Vector2& Entity::GetPosition() const
{
  if (m_transform == m_transformWeak.lock())
  {
    return m_transform->position;
  }
  // Regreso una versión estática de un Vector2 para evitar mover una copia del objeto
  // en teoría nunca debería llegar aquí, pero es mejor prevenir que lamentar
  return Vector2::ZERO;
}

