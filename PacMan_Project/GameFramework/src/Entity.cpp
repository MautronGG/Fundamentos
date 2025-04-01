#include "Component.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Tile.h"

Entity::Entity()
{
  m_components.push_back(std::make_shared<Transform>());
}

void Entity::Update(/*DeltaTime*/const std::vector<std::shared_ptr<Tile>>& tiles)
{
  for (const auto& component : m_components)
  {
    component->Update(this/*DeltaTime*/);
  }
  Move(tiles);
}

bool Entity::CheckNextTile(const std::vector<std::shared_ptr<Tile>>& tiles, Vector3& nexTile)
{
  nexTile.index = (nexTile.y * 28) + nexTile.x;

  m_tileTransformWeak = tiles[nexTile.index]->GetComponent<Transform>();
  m_tileTransform = m_tileTransformWeak.lock();

  nexTile = tiles[nexTile.index]->GetInfo();

  if (nexTile.index >= 0 && nexTile.index < static_cast<int>(tiles.size()) && nexTile.type != 0)
  {
    if (m_isPlayer)
    {
      if (nexTile.type != 4)
      {
        if (nexTile.type == 2 || nexTile.type == 3)
        {
          tiles[nexTile.index]->SetType(1);
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