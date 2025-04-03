#pragma once 

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Component;
class Transform;

struct Vector3
{
  int type = 0;
  int index = 0;
  float x = 0.0f;
  float y = 0.0f;
};

class Entity
{
public:
  Entity();
  virtual ~Entity() = default;

  template <typename T = Component, typename... Args>
  std::weak_ptr<T> AddComponent(Args&&... args);

  template <typename T>
  std::weak_ptr<T> GetComponent() const;
  const Vector2& Entity::GetPosition() const;

  virtual void Update(/*DeltaTime*/);
  
  std::weak_ptr<Transform> m_transformWeak;
  std::shared_ptr<Transform> m_transform;

private:
  std::vector<std::shared_ptr<Component>> m_components;

};

template <typename T, typename... Args>
std::weak_ptr<T> Entity::AddComponent(Args&&... args)
{
  std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
  m_components.push_back(component);
  return component;
}

template <typename T>
std::weak_ptr<T> Entity::GetComponent() const
{
  for (const auto& component : m_components)
  {
    if (std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component))
    {
      return castedComponent;
    }
  }
  return std::weak_ptr<T>();
}