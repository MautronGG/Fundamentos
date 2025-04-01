#include <iostream>
#include <memory>
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
#include "Pac.h"
#include "GameManager.h"

const std::string ASSETS_PATH = "C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Assets/";

typedef void (*loadModFunc)();
enum class Direction;
struct Vector3;
enum class GameState;

void MainMenu(Scene& menuScene, sf::RenderWindow& window);
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles);
void PauseMenu();
void GameOver();

int App();

void UpdateScene(Scene& scene, const std::vector<std::shared_ptr<Tile>>& tiles);
void RenderScene(const Scene& scene, sf::RenderWindow& window);

bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles);
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color);
//bool CheckNextTile(bool isPLayer, Vector3& nextTile, const std::vector<std::shared_ptr<Tile>> tiles, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform);

//void MovePlayer(bool& isMoving, Vector3& tryNextTile, Vector3& nextTile, Direction& direction, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform, std::shared_ptr<Transform>& playerTransform, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset);

//int GetRandomNumber(int min, int max);
//void MoveEnemy(std::vector<std::shared_ptr<Ghost>>& enemiesArray, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset);

void Respawn(std::shared_ptr <Pac> player, std::vector<std::shared_ptr<Ghost>>& enemiesArray);
//void loadMods(const Scene& scene, const Entity* player);

int main()
{
  App();
}

int App()
{
  auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project", sf::State::Fullscreen);
  window.setFramerateLimit(144);

  Scene menuScene;
  Scene gameScene;

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

  std::shared_ptr<Pac> player = std::make_shared<Pac>();
  player->AddComponent<GraphicsComponent>(pacManTexture);

  std::shared_ptr<Entity> level = std::make_shared<Entity>();
  level->AddComponent<GraphicsComponent>(levelTexture);

  Direction playerDirection = Direction::Right;

  std::weak_ptr<Transform> levelTransformWeak = level->GetComponent<Transform>();
  std::shared_ptr<Transform> levelTransform = levelTransformWeak.lock();
  levelTransform->position.x = 480;
  levelTransform->position.y = 0;

  gameScene.AddEntity(level);

  sf::Vector2f offset({ levelTransform->position.x, levelTransform->position.y });
  Map gameMap(offset);

  std::vector<std::shared_ptr<Tile>> tiles = gameMap.GetTiles();

  player->m_startingTile = gameMap.GetStartingTile();

  std::cout << "Starting Tile type: " << player->m_startingTile.type << std::endl;
  std::cout << "Starting Tile Index: " << player->m_startingTile.index << std::endl;
  std::cout << "Starting Tile Position: ( " << player->m_startingTile.x << ", " << player->m_startingTile.y << std::endl;
  
  int tileSize = gameMap.GetTileSize();
  
  player->m_transformWeak = player->GetComponent<Transform>();
  player->m_transform = player->m_transformWeak.lock();

  player->m_tileTransformWeak = tiles[player->m_startingTile.index]->GetComponent<Transform>();
  player->m_tileTransform = player->m_tileTransformWeak.lock();

  player->m_transform->position.x = player->m_tileTransform->position.x + player->m_centerOffset;
  player->m_transform->position.y = player->m_tileTransform->position.y + player->m_centerOffset;

  player->m_nextTile = player->m_startingTile;
  player->m_tryNextTile = player->m_nextTile;

  player->tiles = tiles;
  player->SetStartingTiles(gameMap.GetStartingTile());
  gameScene.AddEntity(player);

  std::vector<std::shared_ptr<Ghost>> enemiesArray;
  Vector3 ghostSTile = gameMap.GetGhostTile();

  for (int i = 0; i < 4; i++)
  {
    std::shared_ptr<Ghost> enemy = std::make_shared<Ghost>();
    enemy->AddComponent<GraphicsComponent>(ghostTexture);

    enemy->m_transformWeak = enemy->GetComponent<Transform>();
    enemy->m_transform = enemy->m_transformWeak.lock();


    enemy->m_startingTile = ghostSTile;

    enemy->m_nextTile = enemy->m_startingTile;
    enemy->m_tryNextTile = enemy->m_nextTile;

    enemy->m_tileTransformWeak = tiles[enemy->m_startingTile.index]->GetComponent<Transform>();
    enemy->m_tileTransform = enemy->m_tileTransformWeak.lock();

    enemy->m_transform->position.x = enemy->m_tileTransform->position.x + enemy->m_centerOffset;
    enemy->m_transform->position.y = enemy->m_tileTransform->position.y + enemy->m_centerOffset;

    enemy->tiles = tiles;
    enemy->m_direction = Direction::Up;

    enemy->SetStartingTiles(gameMap.GetGhostTile());
    enemiesArray.push_back(enemy);
    gameScene.AddEntity(enemy);
  }

  GameManager& manager = GameManager::instance();
  //Scene scene;

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

    switch (manager.GetGameState())
    {
      case GameState::MainMenu:
        MainMenu(menuScene, window);
        break;
        //scene = menuScene;
      case GameState::GameLoop:
        GameLoop(player, enemiesArray, gameScene, gameMap, window, manager, tiles);
        break;
        //scene = gameScene;
      case GameState::PauseMenu:
        PauseMenu();
        break;
      case GameState::GameOver:
        GameOver();
        break;
      default:
        break;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
      window.close();
    }
    window.display();
  }
}

void MainMenu(Scene& menuScene, sf::RenderWindow& window)
{
  //UpdateScene(menuScene);
  RenderScene(menuScene, window);
}
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles)
{
  //MovePlayer(playerIsMoving, player->m_tryNextTile, player->m_nextTile, playerDirection, tileTransformWeak, tileTransform, playerTransform, tiles, speed, centerOffset);
  //MoveEnemy(enemiesArray, tiles, speed, centerOffset);
  UpdateScene(gameScene, tiles);
  gameMap.Render(window);
  RenderScene(gameScene, window);

  //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
  //{
  //  std::cout << "Current Position: " << playerTransform->position.x << ", " << playerTransform->position.y << std::endl;
  //}
  sf::FloatRect playerBounds = player->GetComponent<GraphicsComponent>().lock()->GetBounds();

  for each(std::shared_ptr<Ghost> enemy in enemiesArray)
  {
    sf::FloatRect enemyBounds = enemy->GetComponent<GraphicsComponent>().lock()->GetBounds();
    DrawBounds(window, enemyBounds, sf::Color::Red);
    if (const std::optional intersection = playerBounds.findIntersection(enemyBounds))
    {
      player->m_lives--;
      Respawn(player, enemiesArray);
      continue;
      std::cout << "Impact" << "\n";
      if (player->m_lives <= 0)
      {
        manager.SetGameState(GameState::GameOver);
        continue;
      }
    }
  }

  DrawBounds(window, playerBounds, sf::Color::Green);
  // Draw bounding boxes

}
void PauseMenu()
{

}
void GameOver()
{

}

void UpdateScene(Scene& scene, const std::vector<std::shared_ptr<Tile>>& tiles)
{
  for (const auto& entity : scene.GetEntities())
  {
    entity->Update(tiles);
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

//int GetRandomNumber(int min, int max)
//{
//  static std::random_device rd;
//  static std::mt19937 gen(rd());
//  std::uniform_int_distribution<> dis(min, max);
//  return dis(gen);
//}
//void MoveEnemy(std::vector<std::shared_ptr<Ghost>>& enemiesArray, const std::vector<std::shared_ptr<Tile>>& tiles, float& speed, int centerOffset)
//{
//  for each(std::shared_ptr<Ghost> enemy in enemiesArray)
//  {
//    if (!enemy->enemyIsMoving)
//    {
//      enemy->m_tryNextTile = enemy->m_nextTile;
//      int randomValue = GetRandomNumber(0, 17);
//      switch (randomValue)
//      {
//      case 0:
//        enemy->m_tryNextTile.x--;
//        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
//        {
//          enemy->enemyDirection = Direction::Left;
//        }
//        break;
//      case 1:
//        enemy->m_tryNextTile.x++;
//        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
//        {
//          enemy->enemyDirection = Direction::Right;
//        }
//        break;
//      case 2:
//        enemy->m_tryNextTile.y--;
//        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
//        {
//          enemy->enemyDirection = Direction::Up;
//        }
//        break;
//      case 3:
//        enemy->m_tryNextTile.y++;
//        if (CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform))
//        {
//          enemy->enemyDirection = Direction::Down;
//        }
//        break;
//      default:
//        break;
//      }
//
//      enemy->m_tryNextTile = enemy->m_nextTile;
//
//      switch (enemy->enemyDirection)
//      {
//      case Direction::Left:
//        enemy->m_tryNextTile.x--;
//        break;
//      case Direction::Right:
//        enemy->m_tryNextTile.x++;
//        break;
//      case Direction::Up:
//        enemy->m_tryNextTile.y--;
//        break;
//      case Direction::Down:
//        enemy->m_tryNextTile.y++;
//        break;
//      default:
//        break;
//      }
//
//      bool checkAdvance = CheckNextTile(false, enemy->m_tryNextTile, tiles, enemy->ghostTileTransformWeak, enemy->ghostTileTransform);
//      if (checkAdvance)
//      {
//        enemy->enemyIsMoving = true;
//        enemy->m_nextTile = enemy->m_tryNextTile;
//      }
//    }
//
//    if (enemy->enemyIsMoving)
//    {
//      switch (enemy->enemyDirection)
//      {
//      case Direction::Left:
//        enemy->enemyTransform->position.x -= speed;
//        if (static_cast<int>(enemy->enemyTransform->position.x) <= enemy->ghostTileTransform->position.x + centerOffset)
//        {
//          enemy->enemyTransform->position.x = enemy->ghostTileTransform->position.x + centerOffset;
//          enemy->enemyIsMoving = false;
//        }
//        break;
//      case Direction::Right:
//        enemy->enemyTransform->position.x += speed;
//        if (static_cast<int>(enemy->enemyTransform->position.x) >= enemy->ghostTileTransform->position.x + centerOffset)
//        {
//          enemy->enemyTransform->position.x = enemy->ghostTileTransform->position.x + centerOffset;
//          enemy->enemyIsMoving = false;
//        }
//        break;
//      case Direction::Up:
//        enemy->enemyTransform->position.y -= speed;
//        if (static_cast<int>(enemy->enemyTransform->position.y) <= enemy->ghostTileTransform->position.y + centerOffset)
//        {
//          enemy->enemyTransform->position.y = enemy->ghostTileTransform->position.y + centerOffset;
//          enemy->enemyIsMoving = false;
//        }
//        break;
//      case Direction::Down:
//        enemy->enemyTransform->position.y += speed;
//        if (static_cast<int>(enemy->enemyTransform->position.y) >= enemy->ghostTileTransform->position.y + centerOffset)
//        {
//          enemy->enemyTransform->position.y = enemy->ghostTileTransform->position.y + centerOffset;
//          enemy->enemyIsMoving = false;
//        }
//        break;
//      default:
//        break;
//      }
//    }
//  }
//}

void Respawn(std::shared_ptr <Pac> player, std::vector<std::shared_ptr<Ghost>>& enemiesArray)
{
  player->m_nextTile = player->m_startingTile;
  player->m_tryNextTile = player->m_nextTile;

  player->m_tileTransformWeak = player->tiles[player->m_startingTile.index]->GetComponent<Transform>();
  player->m_tileTransform = player->m_tileTransformWeak.lock();

  player->m_transform->position.x = player->m_tileTransform->position.x + player->m_centerOffset;
  player->m_transform->position.y = player->m_tileTransform->position.y + player->m_centerOffset;

  player->m_direction = Direction::Right;

  for each(std::shared_ptr<Ghost> enemy in enemiesArray)
  {
    enemy->m_nextTile = enemy->m_startingTile;
    enemy->m_tryNextTile = enemy->m_nextTile;

    enemy->m_tileTransformWeak = player->tiles[enemy->m_startingTile.index]->GetComponent<Transform>();
    enemy->m_tileTransform = enemy->m_tileTransformWeak.lock();

    enemy->m_transform->position.x = enemy->m_tileTransform->position.x + player->m_centerOffset;
    enemy->m_transform->position.y = enemy->m_tileTransform->position.y + player->m_centerOffset;

    enemy->m_direction = Direction::Up;
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
