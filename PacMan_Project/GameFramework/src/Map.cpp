#include "Map.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::array<int, 28 * 31> Map::levelLayout = {};

Map::Map(sf::Vector2f offset)
{
  ReadMap("../../../../Resources/map.txt");
  Reset(offset);
}

void Map::Update()
{
  // Update logic if needed
}

void Map::Render(sf::RenderWindow& window)
{
  for (const auto& tile : m_gameTiles)
  {
    tile->Render(window);
  }
}

void Map::ReadMap(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open map file");
  }

  std::string line;
  int index = 0;
  while (std::getline(file, line, ','))
  {
    std::istringstream iss(line);
    int value;
    while (iss >> value)
    {
      levelLayout[index++] = value;
    }
  }
  //std::cout << "LevelLayout: ";
  //for (const auto& value : levelLayout)
  //{
  //  std::cout << value << " ";
  //}
  //std::cout << std::endl;
}
void Map::SaveMap()
{
  std::vector<int> newMap;
  for (int i = 0; i < m_gameTiles.size(); ++i)
  {
    newMap.push_back(m_gameTiles[i]->GetInfo().type);
  }
  std::string filename = "../../../../Resources/savedMap.txt";
   
  std::ofstream file(filename, std::ios::trunc);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open map file for writing");
  }
  for (const auto& value : newMap)
  {
    file << value << ",";
  }
  file.close();
}

void Map::Reset(sf::Vector2f offset)
{
  m_gameTiles.clear();
  for (int y = 0; y < 31; ++y)
  {
    for (int x = 0; x < 28; ++x)
    {
      int index = y * 28 + x;
      int type = levelLayout[index];

      m_gameTiles.push_back(std::make_shared<Tile>(type, x, y, tileSize, offset, index));
      if (type == 6)
      {
        m_playerStartingTile = m_gameTiles[index]->GetInfo();
      }
      else if (type == 7)
      {
        m_ghostStartingTile = m_gameTiles[index]->GetInfo();
      }
    }
  }
}
