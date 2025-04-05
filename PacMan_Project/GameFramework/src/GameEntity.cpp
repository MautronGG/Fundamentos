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
    return true;
  }
  else
  {
    return false;
  }
}