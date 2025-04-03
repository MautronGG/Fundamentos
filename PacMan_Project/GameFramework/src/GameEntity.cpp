#include "GameEntity.h"

void GameEntity::Update()
{
  Entity::Update();
  Move(m_tiles);
}

void GameEntity::SetStartingTiles(Vector3 start)
{
    m_nextTile = start;
    m_tryNextTile = start;
    m_startingTile = start;
}

bool GameEntity::CheckNextTile(Vector3& nexTile)
{
  nexTile.index = (nexTile.y * 28) + nexTile.x;

  m_tileTransformWeak = m_tiles[nexTile.index]->GetComponent<Transform>();
  m_tileTransform = m_tileTransformWeak.lock();

  nexTile = m_tiles[nexTile.index]->GetInfo();

  if (nexTile.index >= 0 && nexTile.index < static_cast<int>(m_tiles.size()) && nexTile.type != 0)
  {
    if (m_isPlayer)
    {
      if (nexTile.type != 4)
      {
        if (nexTile.type == 2 || nexTile.type == 3)
        {
          m_tiles[nexTile.index]->SetType(1);
        }
        return true;
      }
      else
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}