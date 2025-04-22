#pragma once 

#include <vector>
#include <memory>

#include "Entity.h"
#include "Tile.h"
#include "Map.h"

struct Vector3;

enum class Direction { Left, Right, Up, Down };

class GameEntity : public Entity
{
public:
  virtual void Move(const std::vector<std::shared_ptr<Tile>>& tiles) {}
  virtual bool CheckNextTile(Vector3& nexTile);
  void SetStartingTiles(Vector3 start);
  virtual void Reset(const Map& gameMap, const std::vector<std::shared_ptr<Tile>>& tiles, bool isNewGame) {}
  void SetNextTile(Vector3 nextTile)
  {
    m_nextTile = nextTile;
  }
  void SetTryNextTile(Vector3 tryNextTile)
  {
    m_tryNextTile = tryNextTile;
  }

  void SetTiles(const std::vector<std::shared_ptr<Tile>>& tiles)
  {
    m_tiles = tiles;
  }
  
  void Update(/*DeltaTime*/) override;
  
  Direction m_direction = Direction::Right;
  bool m_isMoving = false;
  float m_speed = 2.0f;
  bool m_isPlayer = false;

  const Vector3& GetNextTile() const
  {
    return m_nextTile;
  }

  const Vector3& GetTryNextTile() const
  {
    return m_tryNextTile;
  }
  
  const Vector3& GetStartingTile() const
  {
    return m_startingTile;
  }

  const std::vector<std::shared_ptr<Tile>>& GetTiles() const
  {
    return m_tiles;
  }

  std::weak_ptr<Transform> GetTileTransform() const
  {
    return m_tileTransform;
  }

  int GetCenterOffset() const
  {
    return m_centerOffset;
  }


  private: 
  //TOdo esto debe ser private, no debe ser público
  // no es necesario tener un weak_ptr aquí, el weak se crea a través del shared
  //std::weak_ptr<Transform> m_tileTransformWeak;
  std::shared_ptr<Transform> m_tileTransform;

  Vector3 m_nextTile;
  Vector3 m_tryNextTile;
  Vector3 m_startingTile;

  std::vector<std::shared_ptr<Tile>> m_tiles;
  int m_centerOffset = 0;

};