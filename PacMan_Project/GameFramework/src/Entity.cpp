#include "Component.h"
#include "Entity.h"
#include "TransformComponent.h"

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