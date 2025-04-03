#include "Ghost.h"
#include "Tile.h"
#include <random>


int Ghost::GetRandomNumber(int min, int max)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

void Ghost::Move(const std::vector<std::shared_ptr<Tile>>& tiles)
{
  //bool checkAdvance = false;
  if (!m_isMoving)
  {
    m_tryNextTile = m_nextTile;
    int randomValue = GetRandomNumber(0, 17);
    switch (randomValue)
    {
    case 0:
      m_tryNextTile.x--;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Left;
        //checkAdvance = true;
      }
      break;
    case 1:
      m_tryNextTile.x++;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Right;
        //checkAdvance = true;
      }
      break;
    case 2:
      m_tryNextTile.y--;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Up;
        //checkAdvance = true;
      }
      break;
    case 3:
      m_tryNextTile.y++;
      if (CheckNextTile(m_tryNextTile))
      {
        m_direction = Direction::Down;
        //checkAdvance = true;
      }
      break;
    default:
      //checkAdvance = true;
      break;
    }
    m_tryNextTile = m_nextTile;
  
    switch (m_direction)
    {
    case Direction::Left:
      m_tryNextTile.x--;
      break;
    case Direction::Right:
      m_tryNextTile.x++;
      break;
    case Direction::Up:
      m_tryNextTile.y--;
      break;
    case Direction::Down:
      m_tryNextTile.y++;
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