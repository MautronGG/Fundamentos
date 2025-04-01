#pragma once

#include "Entity.h"

class Pac : public Entity
{
public:
  Pac();
  ~Pac() = default;

  void Move(const std::vector<std::shared_ptr<Tile>>& tiles) override;

  int m_lives = 3;
};