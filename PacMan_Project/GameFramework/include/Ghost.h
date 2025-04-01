#pragma once

#include "Entity.h"

class Ghost : public Entity
{
public:
  Ghost() = default;
  ~Ghost() = default;

  void Move(const std::vector<std::shared_ptr<Tile>>& tiles) override;
  int GetRandomNumber(int min, int max);
};