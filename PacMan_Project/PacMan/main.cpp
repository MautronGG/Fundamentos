#include <iostream>
#include <memory>
#include <optional>
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

#ifdef _WIN32
#include <windows.h>
using DynamicLibraryHandle = HINSTANCE;

inline DynamicLibraryHandle LoadDynamicLibrary(const std::string& path) {
    return LoadLibraryEx(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
}

inline void* GetSymbol(DynamicLibraryHandle handle, const std::string& symbol) {
    return reinterpret_cast<void*>(GetProcAddress(handle, symbol.c_str()));
}

inline void UnloadDynamicLibrary(DynamicLibraryHandle handle) {
    if (!FreeLibrary(handle)) {
        std::cerr << "Failed to unload library: " << GetLastError() << std::endl;
    }
}
#else // Linux
#include <dlfcn.h>
using DynamicLibraryHandle = void*;

DynamicLibraryHandle LoadDynamicLibrary(const std::string& path) {
    return dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
}

void* GetSymbol(DynamicLibraryHandle handle, const std::string& symbol) {
    return dlsym(handle, symbol.c_str());
}

void UnloadDynamicLibrary(DynamicLibraryHandle handle) {
    if (dlclose(handle) != 0) {
        std::cerr << "Failed to unload library: " << dlerror() << std::endl;
    }
}
#endif // end _WIN32

const std::string ASSETS_PATH = "../../../../Assets/";

typedef void (*ComponentScript)(const Entity* parent);
typedef ComponentScript(*LoadModFunc)(); 
void LoadMods(const Scene& scene, Entity* player);

enum class Direction;
struct Vector3;
enum class GameState;

int App();

void MenuMain(Scene& menuScene, sf::RenderWindow& window);
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles, std::shared_ptr<UIEntity> gameScore, std::shared_ptr<UIEntity> gameLives, sf::Text textObject);
void PauseMenu(Scene& gameScene, Scene& pauseScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager);
void GameOver(Scene& gameOverScene, sf::RenderWindow& window, GameManager& manager);
void Restart(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew);

void UpdateScene(Scene& scene);
void RenderScene(const Scene& scene, sf::RenderWindow& window);

bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles);
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color);
void Respawn(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray);
void SaveGame(Map& gameMap ,std::shared_ptr<Pac>& player);
void LoadGame(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew);
void NewGame(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew);


int main()
{
  App();
}

int App()
{
  auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "PacMan Project", sf::State::Windowed);
  window.setFramerateLimit(144);

  sf::Font font;
  if (!font.openFromFile("../../../../Resources/Pixeled.ttf"))
  {
    std::cerr << "Failed to load Pixeled.ttf" << std::endl;
    return -1;
  }
  sf::Text textObject(font);

  Scene menuScene;
  Scene gameScene;
  Scene pauseScene;
  Scene gameOverScene;

  //MainMenuScene
    std::shared_ptr<MainMenu> menu = std::make_shared<MainMenu>();

  std::shared_ptr<UIEntity> title = std::make_shared<UIEntity>(ButtonType::None, UIType::Title);
  title->SetShape(100, "PacMan", { 960, 200 }, textObject);
  title->AddComponent<GraphicsComponent>(title->m_texture);
  menu->AddButton(title);

  std::shared_ptr<UIEntity> startButton = std::make_shared<UIEntity>(ButtonType::Start, UIType::Button);
  startButton->SetShape(50, "Start", { 960, 500 }, textObject);
  startButton->AddComponent<GraphicsComponent>(startButton->m_texture);
  menu->AddButton(startButton);

  std::shared_ptr<UIEntity> continueButton = std::make_shared<UIEntity>(ButtonType::Continue, UIType::Button);
  continueButton->SetShape(50, "Continue", { 960, 700 }, textObject);
  continueButton->AddComponent<GraphicsComponent>(continueButton->m_texture);
  menu->AddButton(continueButton);

  std::shared_ptr<UIEntity> exitButton = std::make_shared<UIEntity>(ButtonType::Exit, UIType::Button);
  exitButton->SetShape(50, "Exit", { 960, 900 }, textObject);
  exitButton->AddComponent<GraphicsComponent>(exitButton->m_texture);
  menu->AddButton(exitButton);

  menuScene.AddEntity(menu);

  for each(std::shared_ptr<UIEntity> entity in menu->m_buttonArray)
    {
      menuScene.AddEntity(entity);
    }

  //GameScene
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

  sf::Vector2f offset({ 489, 17.5 });
  Map gameMap(offset);

  std::vector<std::shared_ptr<Tile>> tiles = gameMap.GetTiles();
  std::cout << "Tiles direction: " << &tiles[0] << std::endl;
  std::cout << "GameMap Tiles direction: " << &gameMap.GetTiles()[0] << std::endl;

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

  std::shared_ptr<UIEntity> gameScore = std::make_shared<UIEntity>(ButtonType::None, UIType::Normal);
  gameScore->SetShape(90, "Score: " + player->GetScore(), { 200, 100 }, textObject);
  gameScore->AddComponent<GraphicsComponent>(gameScore->m_texture);

  std::shared_ptr<UIEntity> gameLives = std::make_shared<UIEntity>(ButtonType::None, UIType::Normal);
  gameLives->SetShape(90, "Lives: " + player->GetLives(), { 200, 200 }, textObject);
  gameLives->AddComponent<GraphicsComponent>(gameLives->m_texture);

  gameScene.AddEntity(gameScore);
  gameScene.AddEntity(gameLives);
  gameScene.AddEntity(level);
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

  
  //PauseScene
  std::shared_ptr<MainMenu> pause = std::make_shared<MainMenu>();

  std::shared_ptr<UIEntity> pauseTitle = std::make_shared<UIEntity>(ButtonType::None, UIType::Title);
  pauseTitle->SetShape(100, "Pause", { 960, 200 }, textObject);
  pauseTitle->AddComponent<GraphicsComponent>(pauseTitle->m_texture);
  pause->AddButton(pauseTitle);

  std::shared_ptr<UIEntity> pauseStartButton = std::make_shared<UIEntity>(ButtonType::Start, UIType::Button);
  pauseStartButton->SetShape(50, "Continue", { 960, 500 }, textObject);
  pauseStartButton->AddComponent<GraphicsComponent>(pauseStartButton->m_texture);
  pause->AddButton(pauseStartButton);

  std::shared_ptr<UIEntity> pauseContinueButton = std::make_shared<UIEntity>(ButtonType::Continue, UIType::Button);
  pauseContinueButton->SetShape(50, "Restart", { 960, 700 }, textObject);
  pauseContinueButton->AddComponent<GraphicsComponent>(pauseContinueButton->m_texture);
  pause->AddButton(pauseContinueButton);

  std::shared_ptr<UIEntity> pauseExitButton = std::make_shared<UIEntity>(ButtonType::Exit, UIType::Button);
  pauseExitButton->SetShape(50, "Exit", { 960, 900 }, textObject);
  pauseExitButton->AddComponent<GraphicsComponent>(pauseExitButton->m_texture);
  pause->AddButton(pauseExitButton);

  pauseScene.AddEntity(pause);

  for each(std::shared_ptr<UIEntity> entity in pause->m_buttonArray)
  {
    pauseScene.AddEntity(entity);
  }
  
  //GameOverScene
  std::shared_ptr<MainMenu> gameOver = std::make_shared<MainMenu>();

  std::shared_ptr<UIEntity> gameOverTitle = std::make_shared<UIEntity>(ButtonType::None, UIType::Title);
  gameOverTitle->SetShape(100, "Game", { 960, 150 }, textObject);
  gameOverTitle->AddComponent<GraphicsComponent>(gameOverTitle->m_texture);
  gameOver->AddButton(gameOverTitle);

  std::shared_ptr<UIEntity> gameOverTitle2 = std::make_shared<UIEntity>(ButtonType::None, UIType::Title);
  gameOverTitle2->SetShape(100, "Over", { 960, 350 }, textObject);
  gameOverTitle2->AddComponent<GraphicsComponent>(gameOverTitle2->m_texture);
  gameOver->AddButton(gameOverTitle2);

  std::shared_ptr<UIEntity> gameOverContinueButton = std::make_shared<UIEntity>(ButtonType::Continue, UIType::Button);
  gameOverContinueButton->SetShape(50, "Restart", { 960, 700 }, textObject);
  gameOverContinueButton->AddComponent<GraphicsComponent>(gameOverContinueButton->m_texture);
  gameOver->AddButton(gameOverContinueButton);

  std::shared_ptr<UIEntity> gameOverExitButton = std::make_shared<UIEntity>(ButtonType::Exit, UIType::Button);
  gameOverExitButton->SetShape(50, "Exit", { 960, 900 }, textObject);
  gameOverExitButton->AddComponent<GraphicsComponent>(gameOverExitButton->m_texture);
  gameOver->AddButton(gameOverExitButton);

  gameOverScene.AddEntity(gameOver);

  for each(std::shared_ptr<UIEntity> entity in gameOver->m_buttonArray)
  {
    gameOverScene.AddEntity(entity);
  }

  LoadMods(gameScene, player.get());
  GameManager& manager = GameManager::instance();

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
        GameLoop(player, enemiesArray, gameScene, gameMap, window, manager, tiles, gameScore, gameLives, textObject);
        break;
        //scene = gameScene;
      case GameState::PauseMenu:
        PauseMenu(gameScene, pauseScene, gameMap, window, manager);
        break;
      case GameState::GameOver:
        GameOver(gameOverScene, window, manager);
        break;
      case GameState::Restart:
        NewGame(player, enemiesArray, gameMap, manager, tiles, offset, true);
        break;
      case GameState::Exit:
        window.close();
        break;
      case GameState::Save:
        SaveGame(gameMap, player);
        manager.SetGameState(GameState::MainMenu);
        break;
      case GameState::Load:
        LoadGame(player, enemiesArray, gameMap, manager,tiles, offset, false);
        manager.SetGameState(GameState::GameLoop);
        break;
      default:
        break;
    }
    window.display();
  }
}


void MenuMain(Scene& menuScene, sf::RenderWindow& window)
{
  UpdateScene(menuScene);
  RenderScene(menuScene, window);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
  {
    window.close();
  }
}
void GameLoop(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Scene& gameScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager, const std::vector<std::shared_ptr<Tile>>& tiles, std::shared_ptr<UIEntity> gameScore, std::shared_ptr<UIEntity> gameLives, sf::Text textObject)
{
  //MovePlayer(playerIsMoving, player->m_tryNextTile, player->m_nextTile, playerDirection, tileTransformWeak, tileTransform, playerTransform, tiles, speed, centerOffset);
  //MoveEnemy(enemiesArray, tiles, speed, centerOffset);
  gameScore->UpdateText(30, "Score: " + std::to_string(player->GetScore()), textObject);
  gameLives->UpdateText(30, "Lives: " + std::to_string(player->GetLives()), textObject);
  UpdateScene(gameScene);
  gameMap.Render(window);
  RenderScene(gameScene, window);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
  {
    manager.SetGameState(GameState::PauseMenu);
  }

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
void PauseMenu(Scene& gameScene, Scene& pauseScene, Map& gameMap, sf::RenderWindow& window, GameManager& manager)
{
  gameMap.Render(window);
  RenderScene(gameScene, window);
  RenderScene(pauseScene, window);
  UpdateScene(pauseScene);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
  {
    manager.SetGameState(GameState::PauseMenu);
  }
}
void GameOver(Scene& gameOverScene, sf::RenderWindow& window, GameManager& manager)
{
  RenderScene(gameOverScene, window);
  UpdateScene(gameOverScene);
}
void Restart(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew)
{
  gameMap.Reset(offset);
  tiles = gameMap.GetTiles();
  player->Reset(gameMap, tiles, isNew);
  for (auto& enemy : enemiesArray)
  {
    enemy->Reset(gameMap, tiles, isNew);
  }
  manager.SetGameState(GameState::GameLoop);
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

void Respawn(std::shared_ptr <Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray)
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
void LoadMods(const Scene& scene, Entity* player)
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

      DynamicLibraryHandle handle = LoadDynamicLibrary(filePath.string().c_str());
      if (nullptr == handle)
      {
        #ifdef _WIN32
        std::cerr << "Cannot open library: " << GetLastError() << std::endl;
        #else
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        #endif
        continue;
      }

      LoadModFunc loadMod = reinterpret_cast<LoadModFunc>(GetSymbol(handle, "loadMod"));
      if (nullptr == loadMod)
      {
        #ifdef _WIN32
        std::cerr << "Cannot load symbol print_dllRuntime: " << GetLastError() << std::endl;
        #else
        std::cerr << "Cannot load symbol print_dllRuntime: " << dlerror() << std::endl;
        #endif
        UnloadDynamicLibrary(handle);
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

        UnloadDynamicLibrary(handle);
        continue;
      }
    }
  }
}
void SaveGame(Map& gameMap, std::shared_ptr <Pac>& player)
{
  gameMap.SaveMap();
  player->SaveData();
}
void LoadGame(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew)
{
  gameMap.ReadMap("../../../../Resources/savedMap.txt");
  player->LoadData();
  Restart(player, enemiesArray, gameMap, manager, tiles, offset, isNew);
}
void NewGame(std::shared_ptr<Pac>& player, std::vector<std::shared_ptr<Ghost>>& enemiesArray, Map& gameMap, GameManager& manager, std::vector<std::shared_ptr<Tile>>& tiles, sf::Vector2f offset, bool isNew)
{
  gameMap.ReadMap("../../../../Resources/map.txt");
  Restart(player, enemiesArray, gameMap, manager, tiles, offset, isNew);
  SaveGame(gameMap, player);
}
