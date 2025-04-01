#include "Map.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::array<int, 28 * 31> Map::levelLayout = {};

Map::Map(sf::Vector2f offset)
{
  ReadMap("C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Resources/map.txt");
  for (int y = 0; y < 31; ++y)
  {
    for (int x = 0; x < 28; ++x)
    {
      int index = y * 28 + x;
      int type = levelLayout[index];
      
      tiles.push_back(std::make_shared<Tile>(type, x, y , tileSize, offset, index));
      if (type == 6)
      {
        m_playerStartingTile = tiles[index]->GetInfo();
      }
      else if (type == 7)
      {
        m_ghostStartingTile = tiles[index]->GetInfo();
      }
    }
  }
}

void Map::Update()
{
  // Update logic if needed
}

void Map::Render(sf::RenderWindow& window)
{
  for (const auto& tile : tiles)
  {
    //std::weak_ptr<Transform> tileTransformWeak = tile->GetComponent<Transform>();
    //std::shared_ptr<Transform> tileTransform = tileTransformWeak.lock();

    //sf::RectangleShape shape = m_shape;
    //
    //shape.setPosition({ tileTransform->position.x + offset.x, tileTransform->position.y + offset.y });
    //
    //shape.setSize(sf::Vector2f(tileSize, tileSize));
    //
    //shape.setFillColor(sf::Color::Black);
    //window.draw(shape);


    //sf::Vector2f center({tileTransform->position.x + offset.x, tileTransform->position.y + offset.y });

    tile->Render(window);
   //shape.setPosition({ tileTransform->position.x + offset.x, tileTransform->position.y + offset.y });
   //shape.setSize(sf::Vector2f(tileSize, tileSize));
   //shape.setFillColor(sf::Color::Transparent);
   //shape.setOutlineColor(sf::Color::Red);
   //window.draw(shape);
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
  std::cout << "LevelLayout: ";
  for (const auto& value : levelLayout)
  {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}