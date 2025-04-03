#include "Tile.h"

Tile::Tile(int type, float x, float y, int tileSize, const sf::Vector2f& offset, const int index)
  : m_tileInfo({ type, index, x, y })
{
  m_shape.setSize(sf::Vector2f(tileSize, tileSize));
  m_shape.setOrigin(m_shape.getGlobalBounds().getCenter());
  std::weak_ptr<Transform> tileTransformWeak = this->GetComponent<Transform>();
  std::shared_ptr<Transform> tileTransform = tileTransformWeak.lock();

  tileTransform->position.x = (x * tileSize) + offset.x;
  tileTransform->position.y = (y * tileSize) + offset.y;

  SetShape();
}
void Tile::Render(sf::RenderWindow& window)
{
  std::weak_ptr<Transform> tileTransformWeak = this->GetComponent<Transform>();
  std::shared_ptr<Transform> tileTransform = tileTransformWeak.lock();

  m_shape.setPosition({ tileTransform->position.x,  tileTransform->position.y });
  window.draw(m_shape);
}
void Tile::SetShape()
{
  //m_type = 1;
  switch (m_tileInfo.type)
  {
  case 0: // Wall
    //m_shape.setSize(sf::Vector2f(10, 10));
    m_shape.setFillColor(sf::Color::Blue);
    break;
  case 1: // Floor
    //m_shape.setSize(sf::Vector2f(10, 10));
    m_shape.setFillColor(sf::Color::Black);
    break;
  case 2: // Dot
    //m_shape.setSize(sf::Vector2f(10, 10));
    m_shape.setFillColor(sf::Color::White);
    break;
  case 3: // PowerUp
    //m_shape.setSize(sf::Vector2f(15, 15));
    m_shape.setFillColor(sf::Color::Yellow);
    break;
  case 4: // GhostHouse
    //m_shape.setSize(sf::Vector2f(25, 25));
    m_shape.setFillColor(sf::Color::Red);
    break;
  case 5: // Teleport
    //m_shape.setSize(sf::Vector2f(tileSize, tileSize));
    m_shape.setFillColor(sf::Color::Green);
    break;
  case 6: // Player
    //m_shape.setSize(sf::Vector2f(tileSize, tileSize));
    m_shape.setFillColor(sf::Color::Cyan);
    break;
  default:
    //m_shape.setSize(sf::Vector2f(tileSize, tileSize));
    m_shape.setFillColor(sf::Color::Black); // Unknown
    break;
  }
}
void Tile::SetType(int newType)
{
  m_tileInfo.type = newType;
  SetShape();
}