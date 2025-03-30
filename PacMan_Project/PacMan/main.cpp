#include <iostream>
#include <memory>
#include <random>
#include <optional>
#include <string>
#include <filesystem>
//#include <dlfcn.h>

#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "Map.h"
#include "Ghost.h"

const std::string ASSETS_PATH = "C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Assets/";

typedef void (*loadModFunc)();

enum class Direction { Left, Right, Up, Down };


void UpdateScene(Scene& scene);
void RenderScene(const Scene& scene, sf::RenderWindow& window);
bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles);
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color);
bool CheckNextTile(bool isPLayer, Vector3& nextTile, const std::vector<std::shared_ptr<Tile>> tiles, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform);
void MovePlayer(bool& isMoving, Vector3& tryNextTile, Vector3& nextTile, Direction& direction, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform, std::weak_ptr<Transform>& playerTransformWeak, std::shared_ptr<Transform>& playerTransform, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset);
int GetRandomNumber(int min, int max);
void MoveEnemy(std::vector<std::shared_ptr<Ghost>>& enemiesArray, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset);
//void loadMods(const Scene& scene, const Entity* player);

struct Vector3;

int main()
{
  auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project");
  window.setFramerateLimit(144);

  int lives = 3;
  float speed = 2.0f;

  Direction playerDirection = Direction::Right;

  bool playerIsMoving = false;

  int currentPosX = 0;
  int currentPosY = 0;
  //int currentPos = levelLayout[currentPosX] + (currentPosY * ysize);

  //sf::Texture pacManTexture(ASSETS_PATH + "pacMan.png");
  //sf::Texture ghostTexture(ASSETS_PATH + "ghost.png");
  //sf::Texture levelTexture(ASSETS_PATH + "level.png");

  sf::Texture pacManTexture;
  if (!pacManTexture.loadFromFile(ASSETS_PATH + "pacMan35.png"))
  {
    std::cerr << "Failed to load pacMan.png" << std::endl;
    return -1;
  }

  sf::Texture ghostTexture;
  if (!ghostTexture.loadFromFile(ASSETS_PATH + "ghost35.png"))
  {
    std::cerr << "Failed to load ghost.png" << std::endl;
    return -1;
  }

  sf::Texture levelTexture;
  if (!levelTexture.loadFromFile(ASSETS_PATH + "level.png"))
  {
    std::cerr << "Failed to load level.png" << std::endl;
    return -1;
  }
  std::shared_ptr<Entity> player = std::make_shared<Entity>();
  player->AddComponent<GraphicsComponent>(pacManTexture);

  std::shared_ptr<Entity> level = std::make_shared<Entity>();
  level->AddComponent<GraphicsComponent>(levelTexture);

  std::weak_ptr<Transform> playerTransformWeak = player->GetComponent<Transform>();
  std::shared_ptr<Transform> playerTransform = playerTransformWeak.lock();

  std::weak_ptr<Transform> levelTransformWeak = level->GetComponent<Transform>();
  std::shared_ptr<Transform> levelTransform = levelTransformWeak.lock();
  levelTransform->position.x = 480;
  levelTransform->position.y = 0;

  Scene scene;

  scene.AddEntity(level);
  scene.AddEntity(player);

  sf::Vector2f offset({ levelTransform->position.x, levelTransform->position.y });
  Map gameMap(offset);

  // Create tiles (example)
  std::vector<std::shared_ptr<Tile>> tiles = gameMap.GetTiles();

  int centerOffset = 5;

  Vector3 sTile = gameMap.GetStartingTile();
  std::cout << "Starting Tile type: " << sTile.type << std::endl;
  std::cout << "Starting Tile Index: " << sTile.index << std::endl;
  std::cout << "Starting Tile Position: ( " << sTile.x << ", " << sTile.y << std::endl;

  std::weak_ptr<Transform> tileTransformWeak = tiles[sTile.index]->GetComponent<Transform>();
  std::shared_ptr<Transform> tileTransform = tileTransformWeak.lock();

  playerTransform->position.x = tileTransform->position.x + centerOffset;
  playerTransform->position.y = tileTransform->position.y + centerOffset;

  int tileSize = gameMap.GetTileSize();

  player->m_nextTile = sTile;
  player->m_tryNextTile = player->m_nextTile;

  std::vector<std::shared_ptr<Ghost>> enemiesArray;
  sTile = gameMap.GetGhostTile();
  for (int i = 0; i < 4; i++)
  {
    std::shared_ptr<Ghost> enemy = std::make_shared<Ghost>();
    enemy->AddComponent<GraphicsComponent>(ghostTexture);

    enemy->enemyTransformWeak = enemy->GetComponent<Transform>();
    enemy->enemyTransform = enemy->enemyTransformWeak.lock();

    scene.AddEntity(enemy);

    enemy->m_nextTile = sTile;
    enemy->m_tryNextTile = enemy->m_nextTile;

    enemy->ghostTileTransformWeak = tiles[sTile.index]->GetComponent<Transform>();
    enemy->ghostTileTransform = enemy->ghostTileTransformWeak.lock();

    enemy->enemyTransform->position.x = enemy->ghostTileTransform->position.x + centerOffset;
    enemy->enemyTransform->position.y = enemy->ghostTileTransform->position.y + centerOffset;

    enemy->enemyDirection = Direction::Up;

    enemiesArray.push_back(enemy);
  }

  while (window.isOpen())
  {
    sf::Color screenColor(100, 100, 250);
    window.clear(screenColor);

    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
    }
 
    
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
    //{
    //  std::cout << "Current Position: " << playerTransform->position.x << ", " << playerTransform->position.y << std::endl;
    //}

    sf::FloatRect playerBounds = player->GetComponent<GraphicsComponent>().lock()->GetBounds();
    
    for each(std::shared_ptr<Entity> enemy in enemiesArray)
    {
      sf::FloatRect enemyBounds = enemy->GetComponent<GraphicsComponent>().lock()->GetBounds();
      DrawBounds(window, enemyBounds, sf::Color::Red);
      if (const std::optional intersection = playerBounds.findIntersection(enemyBounds))
      {
        lives--;
        //respawn
        //std::cout << "Impact" << "\n";
        if (lives <= 0)
        {
          //std::cout << "Die" << "\n";
          //restart game
        }
      }
    }

    MovePlayer(playerIsMoving, player->m_tryNextTile, player->m_nextTile, playerDirection, tileTransformWeak, tileTransform, playerTransformWeak, playerTransform, tiles, speed, centerOffset);
    MoveEnemy(enemiesArray, tiles, speed, centerOffset);
    UpdateScene(scene);
    gameMap.Render(window);
    RenderScene(scene, window);

    // Draw bounding boxes
    DrawBounds(window, playerBounds, sf::Color::Green);

    window.display();
  }
}

void UpdateScene(Scene& scene)
{
  for (const auto& entity : scene.GetEntities())
  {
    entity->Update(/*DeltaTime*/);
  }
}
void RenderScene(const Scene& scene, sf::RenderWindow& window)
{
  for (const auto& entity : scene.GetEntities())
  {
    std::weak_ptr<GraphicsComponent> graphicComponentWeak = entity->GetComponent<GraphicsComponent>();

    if (const std::shared_ptr<GraphicsComponent> graphicComponent = graphicComponentWeak.lock())
    {
      const sf::Sprite& sprite = graphicComponent->GetSprite();
      window.draw(sprite);
    }
  }
}
bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles)
{
  for (const auto& tile : tiles)
  {
    if (tile->GetInfo().type == 0 && playerBounds.findIntersection(tile->GetBounds()))
    {
      return true;
    }
  }
  return false;
}
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color)
{
  sf::RectangleShape rectangle;
  rectangle.setPosition(bounds.position);
  rectangle.setSize(sf::Vector2f(bounds.size));
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(color);
  rectangle.setOutlineThickness(1.0f);
  window.draw(rectangle);
}
bool CheckNextTile(bool isPlayer, Vector3& nextTile, const std::vector<std::shared_ptr<Tile>> tiles, std::weak_ptr<Transform>& tileTransformWeak,  std::shared_ptr<Transform>& tileTransform)
{
  nextTile.index = (nextTile.y * 28) + nextTile.x;

  tileTransformWeak = tiles[nextTile.index]->GetComponent<Transform>();
  tileTransform = tileTransformWeak.lock();

  nextTile = tiles[nextTile.index]->GetInfo();

  if (nextTile.index >= 0 && nextTile.index < static_cast<int>(tiles.size()) && nextTile.type != 0)
  {
    if (isPlayer)
    {
      if (nextTile.type != 4)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  else
  {
    return false;
  }
}
void MovePlayer(bool& isMoving, Vector3& tryNextTile, Vector3& nextTile, Direction& direction, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform, std::weak_ptr<Transform>& playerTransformWeak, std::shared_ptr<Transform>& playerTransform, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset)
{
  if (!isMoving)
  {
    tryNextTile = nextTile;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
      tryNextTile.x--;
      if (CheckNextTile(true, tryNextTile, tiles, tileTransformWeak, tileTransform))
      {
        direction = Direction::Left;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
      tryNextTile.x++;
      if (CheckNextTile(true, tryNextTile, tiles, tileTransformWeak, tileTransform))
      {
        direction = Direction::Right;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
      //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      tryNextTile.y--;
      if (CheckNextTile(true, tryNextTile, tiles, tileTransformWeak, tileTransform))
      {
        direction = Direction::Up;
      }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
      tryNextTile.y++;
      if (CheckNextTile(true, tryNextTile, tiles, tileTransformWeak, tileTransform))
      {
        direction = Direction::Down;
      }
    }
    tryNextTile = nextTile;

    switch (direction)
    {
    case Direction::Left:
      tryNextTile.x--;
      //playerTransform->rotation = 180;
      break;
    case Direction::Right:
      tryNextTile.x++;
      //playerTransform->rotation = 0;
      break;
    case Direction::Up:
      tryNextTile.y--;
      //playerTransform->rotation = -90;
      break;
    case Direction::Down:
      tryNextTile.y++;
      //playerTransform->rotation = 90;
      break;
    default:
      break;
    }

    bool checkAdvance = CheckNextTile(true, tryNextTile, tiles, tileTransformWeak, tileTransform);
    if (checkAdvance)
    {
      isMoving = true;
      nextTile = tryNextTile;
    }
  }

  if (isMoving)
  {
    switch (direction)
    {
    case Direction::Left:
      playerTransform->position.x -= speed;
      if (static_cast<int>(playerTransform->position.x) <= tileTransform->position.x + centerOffset)
      {
        playerTransform->position.x = tileTransform->position.x + centerOffset;
        isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Right:
      playerTransform->position.x += speed;
      if (static_cast<int>(playerTransform->position.x) >= tileTransform->position.x + centerOffset)
      {
        playerTransform->position.x = tileTransform->position.x + centerOffset;
        isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Up:
      playerTransform->position.y -= speed;
      if (static_cast<int>(playerTransform->position.y) <= tileTransform->position.y + centerOffset)
      {
        playerTransform->position.y = tileTransform->position.y + centerOffset;
        isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    case Direction::Down:
      playerTransform->position.y += speed;
      if (static_cast<int>(playerTransform->position.y) >= tileTransform->position.y + centerOffset)
      {
        playerTransform->position.y = tileTransform->position.y + centerOffset;
        isMoving = false;
        //std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index:" << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
      }
      break;
    default:
      break;
    }
  }
}

int GetRandomNumber(int min, int max)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}
void MoveEnemy(std::vector<std::shared_ptr<Ghost>>& enemiesArray, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset)
{
  for each(std::shared_ptr<Ghost> enemy in enemiesArray)
  {
    if (!enemy->enemyIsMoving)
    {
      enemy->m_tryNextTile = enemy->m_nextTile;
      int randomValue = GetRandomNumber(0, 17);
      switch (randomValue)
      {
      case 0:
        enemy->m_tryNextTile.x--;
        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
        {
          enemy->enemyDirection = Direction::Left;
        }
        break;
      case 1:
        enemy->m_tryNextTile.x++;
        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
        {
          enemy->enemyDirection = Direction::Right;
        }
        break;
      case 2:
        enemy->m_tryNextTile.y--;
        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
        {
          enemy->enemyDirection = Direction::Up;
        }
        break;
      case 3:
        enemy->m_tryNextTile.y++;
        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
        {
          enemy->enemyDirection = Direction::Down;
        }
        break;
      default:
        break;
      }

      enemy->m_tryNextTile = enemy->m_nextTile;

      switch (enemy->enemyDirection)
      {
      case Direction::Left:
        enemy->m_tryNextTile.x--;
        break;
      case Direction::Right:
        enemy->m_tryNextTile.x++;
        break;
      case Direction::Up:
        enemy->m_tryNextTile.y--;
        break;
      case Direction::Down:
        enemy->m_tryNextTile.y++;
        break;
      default:
        break;
      }

      bool checkAdvance = CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform);
      if (checkAdvance)
      {
        enemy->enemyIsMoving = true;
        enemy->m_nextTile = enemy->m_tryNextTile;
      }
    }

    if (enemy->enemyIsMoving)
    {
      switch (enemy->enemyDirection)
      {
      case Direction::Left:
        enemy->enemyTransform->position.x -= speed;
        if (static_cast<int>(enemy->enemyTransform->position.x) <= enemy->ghostTileTransform->position.x + centerOffset)
        {
          enemy->enemyTransform->position.x = enemy->ghostTileTransform->position.x + centerOffset;
          enemy->enemyIsMoving = false;
        }
        break;
      case Direction::Right:
        enemy->enemyTransform->position.x += speed;
        if (static_cast<int>(enemy->enemyTransform->position.x) >= enemy->ghostTileTransform->position.x + centerOffset)
        {
          enemy->enemyTransform->position.x = enemy->ghostTileTransform->position.x + centerOffset;
          enemy->enemyIsMoving = false;
        }
        break;
      case Direction::Up:
        enemy->enemyTransform->position.y -= speed;
        if (static_cast<int>(enemy->enemyTransform->position.y) <= enemy->ghostTileTransform->position.y + centerOffset)
        {
          enemy->enemyTransform->position.y = enemy->ghostTileTransform->position.y + centerOffset;
          enemy->enemyIsMoving = false;
        }
        break;
      case Direction::Down:
        enemy->enemyTransform->position.y += speed;
        if (static_cast<int>(enemy->enemyTransform->position.y) >= enemy->ghostTileTransform->position.y + centerOffset)
        {
          enemy->enemyTransform->position.y = enemy->ghostTileTransform->position.y + centerOffset;
          enemy->enemyIsMoving = false;
        }
        break;
      default:
        break;
      }
    }
  }
}
//void LoadMods(const Scene& scene, const Entity* player)
//{
//  std::filesystem::path modPath(ASSETS_PATH + "mods/");
//  if (!std::filesystem::exists(modPath))
//  {
//    std::cout << "No Mods Founds" << "\n";
//    return;
//  }
//  for (auto const& dir_entry : std::filesystem::directory_iterator(modPath))
//  {
//    std::filesystem::path filePath = dir_entry.path();
//    if (filePath.extension() == ".dll" || filePath.extension() == ".so")
//    {
//      std::string modName = filePath.stem().string();
//      std::cout << "loading mods: " << << "\n";
//
//      void* handlke = dlopen(modName.c_str(), RTLD_LAZY);
//      if (nullptr == handle)
//      {
//        std::cerr << "Cannot open library: " << dlerror() << "\n";
//        continue;
//      }
//
//      LoadModFunc loadMod = reinterpret_cast<LoadModFunc>(dlsym(handle));
//      if (nullptr == loadMod)
//      {
//        std::cerr << "Cannot load symbol print_dllRuntime: " << dlerror() << "\n";
//        dlclose(handle);
//        continue;
//      }
//
//      try
//      {
//        loadMod();
//      }
//      catch (std::exception& e)
//      {
//        std::cerr << "Error Loading mod: " << e.what() << "\n";
//
//        dlclose(handle);
//        continue;
//      }
//      dlclose(handle);
//    }
//  }
//}
