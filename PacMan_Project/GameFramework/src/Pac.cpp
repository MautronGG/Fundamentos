#include "Pac.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <fstream>
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

void Pac::Reset(const Map& gameMap, const std::vector<std::shared_ptr<Tile>>& tiles, bool isNewGame)
{
  if (isNewGame)
  {
    m_lives = 3;
    m_score = 0;
  }

  m_tiles = tiles;

  m_startingTile = gameMap.GetStartingTile();
  m_nextTile = m_startingTile;
  m_tryNextTile = m_nextTile;

  m_tileTransformWeak = m_tiles[m_startingTile.index]->GetComponent<Transform>();
  m_tileTransform = m_tileTransformWeak.lock();

  m_transform->position.x = m_tileTransform->position.x + m_centerOffset;
  m_transform->position.y = m_tileTransform->position.y + m_centerOffset;

  m_direction = Direction::Right;
}

void Pac::UpdateScore(int score)
{
  m_score += score;
}

bool Pac::CheckNextTile(Vector3& nexTile)
{
  nexTile.index = (nexTile.y * 28) + nexTile.x;

  m_tileTransformWeak = m_tiles[nexTile.index]->GetComponent<Transform>();
  m_tileTransform = m_tileTransformWeak.lock();

  nexTile = m_tiles[nexTile.index]->GetInfo();

  if (nexTile.index >= 0 && nexTile.index < static_cast<int>(m_tiles.size()) && nexTile.type != 0)
  {
    if (nexTile.type != 4)
    {
      if (nexTile.type == 2 || nexTile.type == 3)
      {
        m_tiles[nexTile.index]->SetType(1);
        UpdateScore(100);
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

void Pac::SaveData()
{
  std::string filename = "../../../../Resources/pacData.txt";
  std::ofstream file(filename, std::ios::trunc); // Open the file in trunc mode to erase its content
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open data file for writing");
  }

  file << m_lives << ",";
  file << m_score;

  file.close();
  std::cout << "Data saved successfully to " << filename << std::endl;
}

void Pac::LoadData()
{
  std::string filename = "../../../../Resources/pacData.txt";
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open data file for reading");
  }
  while (!file.eof()) 
  {
    std::string lives;
    std::getline(file, lives, ',');
    m_lives = static_cast<std::int32_t>(std::stoi(lives));
    std::string score;
    std::getline(file, score, ',');
    m_score = static_cast<std::int32_t>(std::stoi(score));
  }

  file.close();
  std::cout << "Data loaded successfully from " << filename << std::endl;
}