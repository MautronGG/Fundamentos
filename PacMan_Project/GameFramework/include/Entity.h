#pragma once 

#include <vector>
#include <memory>

class Component;
class Transform;
class Tile;

struct Vector3
{
  int type = 0;
  int index = 0;
  float x = 0.0f;
  float y = 0.0f;
};

enum class Direction { Left, Right, Up, Down };

class Entity
{
public:
  Entity();
  virtual ~Entity() = default;

  template <typename T = Component, typename... Args>
  std::weak_ptr<T> AddComponent(Args&&... args);

  template <typename T>
  std::weak_ptr<T> GetComponent() const;

  virtual void Move(const std::vector<std::shared_ptr<Tile>>& tiles) {}
  bool CheckNextTile(const std::vector<std::shared_ptr<Tile>>& tiles, Vector3& nexTile);

  void Update(/*DeltaTime*/const std::vector<std::shared_ptr<Tile>>& tiles);
  

private:
  std::vector<std::shared_ptr<Component>> m_components;
public:
  Direction m_direction = Direction::Right;
  bool m_isMoving = false;
  float m_speed = 2.0f;
  bool m_isPlayer = false;

  std::weak_ptr<Transform> m_tileTransformWeak;
  std::shared_ptr<Transform> m_tileTransform;

  std::weak_ptr<Transform> m_transformWeak;
  std::shared_ptr<Transform> m_transform;

  Vector3 m_nextTile;
  Vector3 m_tryNextTile;
  Vector3 m_startingTile;
  std::vector<std::shared_ptr<Tile>> tiles;
  int m_centerOffset = 5;

  void SetStartingTiles(Vector3 start) {
    m_nextTile = start;
    m_tryNextTile = start;
    m_startingTile = start;
  }

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