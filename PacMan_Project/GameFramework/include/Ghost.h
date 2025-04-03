#pragma once

#include "GameEntity.h"

class Ghost : public GameEntity
{
public:
  Ghost() = default;
  ~Ghost() = default;

  void Move(const std::vector<std::shared_ptr<Tile>>& tiles) override;
  int GetRandomNumber(int min, int max);
};