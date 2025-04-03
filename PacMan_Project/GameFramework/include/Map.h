#pragma once

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Entity.h"

class Tile;

struct Vector3;

class Map
{
public:
  Map(sf::Vector2f offset);
  ~Map() = default;

  void Update();
  void Render(sf::RenderWindow& window);

  inline const std::vector<std::shared_ptr<Tile>>& GetTiles() const
  {
    return tiles;
  }
  inline const Vector3& GetStartingTile() const
  {
    return m_playerStartingTile;
  }
  inline const Vector3& GetGhostTile() const
  {
    return m_ghostStartingTile;
  }
  inline const int& GetTileSize() const
  {
    return tileSize;
  }
  void ReadMap(const std::string& filename);

private:
  //0=Wall
  //1=Floor
  //2=Dot
  //3=PowerUp
  //4=GhostHouse
  //5=Teleport

  static std::array<int, 28 * 31> levelLayout;
  static constexpr int tileSize = 35;
  std::vector<std::shared_ptr<Tile>> tiles;
  sf::RectangleShape m_shape;
  Vector3 m_playerStartingTile;
  Vector3 m_ghostStartingTile;
};