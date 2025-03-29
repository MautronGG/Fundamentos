#include "Map.h"
#include "Tile.h"
#include <iostream>

Map::Map(sf::Vector2f offset)
{
  for (int y = 0; y < 31; ++y)
  {
    for (int x = 0; x < 28; ++x)
    {
      int index = y * 28 + x;
      int type = levelLayout[index];
      
      m_tiles.push_back(std::make_shared<Tile>(type, x, y , tileSize, offset, index));
      if (type == 6)
      {
        m_playerStartingTile = m_tiles[index]->GetInfo();
      }
      else if (type == 7)
      {
        m_ghostStartingTile = m_tiles[index]->GetInfo();
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
  for (const auto& tile : m_tiles)
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