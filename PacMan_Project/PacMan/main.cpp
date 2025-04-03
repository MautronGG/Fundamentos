#include <iostream>
#include <memory>
#include <optional>
#include <windows.h>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Scene.h"
#include "Entity.h"
//#include "GameEntity.h"
#include "Pac.h"
#include "Ghost.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "ScriptComponent.h"
#include "Map.h"
#include "GameManager.h"

const std::string ASSETS_PATH = "C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Assets/";

typedef void (*ComponentScript)(const Entity* parent);
typedef ComponentScript(*LoadModFunc)(); 
void loadMods(const Scene& scene, Entity* player);

enum class Direction;
struct Vector3;
enum class GameState;

int App();

void MenuMain(Scene& menuScene, sf::RenderWindow& window);
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles);
void PauseMenu();
void GameOver();

void UpdateScene(Scene& scene);
void RenderScene(const Scene& scene, sf::RenderWindow& window);

bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles);
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color);
void Respawn(std::shared_ptr <Pac> player, std::vector<std::shared_ptr<Ghost>>& enemiesArray);

int main()
{
  App();
}

int App()
{
  auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project", sf::State::Fullscreen);
  window.setFramerateLimit(144);

  sf::Font font;
  if (!font.openFromFile("C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Resources/Pixeled.ttf"))
  {
    std::cerr << "Failed to load Pixeled.ttf" << std::endl;
    return -1;
  }
  sf::Text textObject(font);
  Scene menuScene;
  MainMenu menu;
  std::shared_ptr<UIEntity> title = std::make_shared<UIEntity>();
  title->SetShape(UIType::Title, 100, "PacMan", { 960, 200 }, textObject);
  title->AddComponent<GraphicsComponent>(title->m_texture);
  menu.AddButton(title);

  std::shared_ptr<UIEntity> startButton = std::make_shared<UIEntity>();
  startButton->SetShape(UIType::Button, 50, "Start", { 960, 500 }, textObject);
  startButton->AddComponent<GraphicsComponent>(startButton->m_texture);
  menu.AddButton(startButton);

  std::shared_ptr<UIEntity> continueButton = std::make_shared<UIEntity>();
  continueButton->SetShape(UIType::Button, 50, "Continue", { 960, 600 }, textObject);
  continueButton->AddComponent<GraphicsComponent>(continueButton->m_texture);
  menu.AddButton(continueButton);

  std::shared_ptr<UIEntity> exitButton = std::make_shared<UIEntity>();
  exitButton->SetShape(UIType::Button, 50, "Exit", { 960, 700 }, textObject);
  exitButton->AddComponent<GraphicsComponent>(exitButton->m_texture);
  menu.AddButton(exitButton);

  for each(std::shared_ptr<UIEntity> button in menu.m_buttonArray)
  {
    menuScene.AddEntity(button);
  }

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

  level->m_transformWeak = level->GetComponent<Transform>();
  level->m_transform = level->m_transformWeak.lock();

  level->m_transform->position.x = 960;
  level->m_transform->position.y = 540;

  gameScene.AddEntity(level);

  sf::Vector2f offset({ 489, 17.5});
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

  player->m_tiles = tiles;
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

    enemy->m_tiles = tiles;
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
        MenuMain(menuScene, window);
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


void MenuMain(Scene& menuScene, sf::RenderWindow& window)
{
  UpdateScene(menuScene);
  RenderScene(menuScene, window);
}
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles)
{
  //MovePlayer(playerIsMoving, player->m_tryNextTile, player->m_nextTile, playerDirection, tileTransformWeak, tileTransform, playerTransform, tiles, speed, centerOffset);
  //MoveEnemy(enemiesArray, tiles, speed, centerOffset);
  UpdateScene(gameScene);
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

void UpdateScene(Scene& scene)
{
  for (const auto& entity : scene.GetEntities())
  {
    entity->Update();
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

void Respawn(std::shared_ptr <Pac> player, std::vector<std::shared_ptr<Ghost>>& enemiesArray)
{
  player->m_nextTile = player->m_startingTile;
  player->m_tryNextTile = player->m_nextTile;

  player->m_tileTransformWeak = player->m_tiles[player->m_startingTile.index]->GetComponent<Transform>();
  player->m_tileTransform = player->m_tileTransformWeak.lock();

  player->m_transform->position.x = player->m_tileTransform->position.x + player->m_centerOffset;
  player->m_transform->position.y = player->m_tileTransform->position.y + player->m_centerOffset;

  player->m_direction = Direction::Right;

  for each(std::shared_ptr<Ghost> enemy in enemiesArray)
  {
    enemy->m_nextTile = enemy->m_startingTile;
    enemy->m_tryNextTile = enemy->m_nextTile;

    enemy->m_tileTransformWeak = enemy->m_tiles[enemy->m_startingTile.index]->GetComponent<Transform>();
    enemy->m_tileTransform = enemy->m_tileTransformWeak.lock();

    enemy->m_transform->position.x = enemy->m_tileTransform->position.x + enemy->m_centerOffset;
    enemy->m_transform->position.y = enemy->m_tileTransform->position.y + enemy->m_centerOffset;

    enemy->m_direction = Direction::Up;
  }

}
void loadMods(const Scene& scene, Entity* player)
{
  std::filesystem::path modPath(ASSETS_PATH + "mods/");
  if (!std::filesystem::exists(modPath))
  {
    std::cout << "No mods found" << std::endl;
    return;
  }
  for (auto const& dir_entry : std::filesystem::directory_iterator{ modPath })
  {
    std::filesystem::path filePath = dir_entry.path();
    if (filePath.extension() == ".dll" || filePath.extension() == ".so")
    {
      std::string modName = filePath.stem().string();
      std::cout << "Loading mod: " << modName << std::endl;

      HMODULE handle = LoadLibrary(filePath.string().c_str());
      if (nullptr == handle)
      {
        std::cerr << "Cannot open library: " << GetLastError() << std::endl;
        continue;
      }

      LoadModFunc loadMod = reinterpret_cast<LoadModFunc>(GetProcAddress(handle, "loadMod"));
      if (nullptr == loadMod)
      {
        std::cerr << "Cannot load symbol print_dllRuntime: " << GetLastError() << std::endl;
        FreeLibrary(handle);
        continue;
      }

      try
      {
        ComponentScript componentScript = loadMod();
        if (componentScript)
        {
          std::cout << "Mod loaded successfully" << std::endl;
          player->AddComponent<ScriptComponent>(componentScript);
        }
        else
        {
          std::cerr << "Failed to load mod" << std::endl;
        }
      }
      catch (std::exception& e)
      {
        std::cerr << "Error loading mod: " << e.what() << std::endl;

        FreeLibrary(handle);
        continue;
      }
    }
  }
}
