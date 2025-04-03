#include "Pac.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Pac::Pac()
{
  m_isPlayer = true;
}

void Pac::Move(const std::vector<std::shared_ptr<Tile>>& tiles)
{
  //bool checkAdvance = false;
  if (!m_isMoving)
  {
    m_tryNextTile = m_nextTile;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
      m_tryNextTile.x--;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Left;
        //checkAdvance = true;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
      m_tryNextTile.x++;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Right;
        //checkAdvance = true;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
      m_tryNextTile.y--;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Up;
        //checkAdvance = true;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
      m_tryNextTile.y++;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Down;
        //checkAdvance = true;
      }
    }
    m_tryNextTile = m_nextTile;

    switch (m_direction)
    {
    case Direction::Left:
      m_tryNextTile.x--;
      m_transform->rotation = 180;
      break;
    case Direction::Right:
      m_tryNextTile.x++;
      m_transform->rotation = 0;
      break;
    case Direction::Up:
      m_tryNextTile.y--;
      m_transform->rotation = -90;
      break;
    case Direction::Down:
      m_tryNextTile.y++;
      m_transform->rotation = 90;
      break;
    default:
      break;
    }

    bool checkAdvance = CheckNextTile(m_tryNextTile);
    if (checkAdvance)
    {
      m_isMoving = true;
      m_nextTile = m_tryNextTile;
    }
  }

  if (m_isMoving)
  {
    switch (m_direction)
    {
    case Direction::Left:
      m_transform->position.x -= m_speed;
      if (static_cast<int>(m_transform->position.x) <= m_tileTransform->position.x + m_centerOffset)
      {
        m_transform->position.x = m_tileTransform->position.x + m_centerOffset;
        m_isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Right:
      m_transform->position.x += m_speed;
      if (static_cast<int>(m_transform->position.x) >= m_tileTransform->position.x + m_centerOffset)
      {
        m_transform->position.x = m_tileTransform->position.x + m_centerOffset;
        m_isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Up:
      m_transform->position.y -= m_speed;
      if (static_cast<int>(m_transform->position.y) <= m_tileTransform->position.y + m_centerOffset)
      {
        m_transform->position.y = m_tileTransform->position.y + m_centerOffset;
        m_isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Down:
      m_transform->position.y += m_speed;
      if (static_cast<int>(m_transform->position.y) >= m_tileTransform->position.y + m_centerOffset)
      {
        m_transform->position.y = m_tileTransform->position.y + m_centerOffset;
        m_isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index:" << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    default:
      break;
    }
  }
}