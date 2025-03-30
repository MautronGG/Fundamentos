#pragma once

#include "Entity.h"

enum class Direction;
class Ghost : public Entity
{
public:
  Ghost() = default;
  ~Ghost() = default;

public:
  std::weak_ptr<Transform> ghostTileTransformWeak;
  std::shared_ptr<Transform> ghostTileTransform;

  std::weak_ptr<Transform> enemyTransformWeak;
  std::shared_ptr<Transform> enemyTransform;

  Direction enemyDirection;

  bool enemyIsMoving = false;

};