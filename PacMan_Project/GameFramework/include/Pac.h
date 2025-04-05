#pragma once

#include "GameEntity.h"

class Pac : public GameEntity
{
public:
  Pac();
  ~Pac() = default;

  void Move(const std::vector<std::shared_ptr<Tile>>& tiles) override;
  void Reset(const Map& gameMap, const std::vector<std::shared_ptr<Tile>>& tiles, bool isNewGame) override;
  bool CheckNextTile(Vector3& nexTile) override;

  void UpdateScore(int score);
  inline const int GetScore() const
  {
    return m_score;
  }
  inline const int GetLives() const
  {
    return m_lives;
  }

  void SaveData();
  void LoadData();

  int m_lives = 3;
  int m_score = 0;
};