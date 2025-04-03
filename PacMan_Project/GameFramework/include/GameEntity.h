#pragma once 

#include <vector>
#include <memory>

#include "Entity.h"
#include "Tile.h"

struct Vector3;

enum class Direction { Left, Right, Up, Down };

class GameEntity : public Entity
{
public:
  virtual void Move(const std::vector<std::shared_ptr<Tile>>& tiles) {}
  bool CheckNextTile(Vector3& nexTile);
  void SetStartingTiles(Vector3 start);

  void Update(/*DeltaTime*/) override;
  
  Direction m_direction = Direction::Right;
  bool m_isMoving = false;
  float m_speed = 2.0f;
  bool m_isPlayer = false;

  std::weak_ptr<Transform> m_tileTransformWeak;
  std::shared_ptr<Transform> m_tileTransform;

  Vector3 m_nextTile;
  Vector3 m_tryNextTile;
  Vector3 m_startingTile;

  std::vector<std::shared_ptr<Tile>> m_tiles;
  int m_centerOffset = 0;

};