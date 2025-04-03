#pragma once

#include "GameEntity.h"

class Pac : public GameEntity
{
public:
  Pac();
  ~Pac() = default;

  void Move(const std::vector<std::shared_ptr<Tile>>& tiles) override;

  int m_lives = 3;
};