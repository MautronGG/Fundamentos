#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "Map.h"

const std::string ASSETS_PATH = "C:/Users/mauig/Desktop/Docs pochos/c++/Fundamentos/PacMan_Project/Assets/";

enum class Direction { None, Left, Right, Up, Down };

void UpdateScene(Scene& scene);
void RenderScene(const Scene& scene, sf::RenderWindow& window);
bool CheckCollision(const sf::FloatRect& playerBounds, const std::vector<std::shared_ptr<Tile>>& tiles);
void DrawBounds(sf::RenderWindow& window, const sf::FloatRect& bounds, const sf::Color& color);
bool CheckNextTile(Direction direction, Vector3& nextTile, const std::vector<std::shared_ptr<Tile>> tiles, std::weak_ptr<Transform>& tileTransformWeak, std::shared_ptr<Transform>& tileTransform);

struct Vector3;

int main()
{
  auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project");
  window.setFramerateLimit(144);

  int lives = 3;
  float speed = 2.0f;
  Direction direction = Direction::Right;
  bool isMoving = false;



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

  std::shared_ptr<Entity> enemy = std::make_shared<Entity>();
  enemy->AddComponent<GraphicsComponent>(ghostTexture);

  std::shared_ptr<Entity> level = std::make_shared<Entity>();
  level->AddComponent<GraphicsComponent>(levelTexture);

  std::weak_ptr<Transform> playerTransformWeak = player->GetComponent<Transform>();
  std::shared_ptr<Transform> playerTransform = playerTransformWeak.lock();

  std::weak_ptr<Transform> enemyTransformWeak = enemy->GetComponent<Transform>();
  std::shared_ptr<Transform> enemyTransform = enemyTransformWeak.lock();

  std::weak_ptr<Transform> levelTransformWeak = level->GetComponent<Transform>();
  std::shared_ptr<Transform> levelTransform = levelTransformWeak.lock();
  levelTransform->position.x = 480;
  levelTransform->position.y = 0;

  Scene scene;

  scene.AddEntity(level);
  scene.AddEntity(enemy);
  scene.AddEntity(player);

  sf::Vector2f offset({ levelTransform->position.x, levelTransform->position.y });
  Map gameMap(offset);

  // Create tiles (example)
  std::vector<std::shared_ptr<Tile>> tiles = gameMap.GetTiles();

  Vector3 sTile = gameMap.GetGhostTile();
  std::weak_ptr<Transform> tileTransformWeak = tiles[sTile.index]->GetComponent<Transform>();
  std::shared_ptr<Transform> tileTransform = tileTransformWeak.lock();

  int centerOffset = 5;
  enemyTransform->position.x = tileTransform->position.x + centerOffset;
  enemyTransform->position.y = tileTransform->position.y + centerOffset;

  sTile = gameMap.GetStartingTile();
  std::cout << "Starting Tile type: " << sTile.type << std::endl;
  std::cout << "Starting Tile Index: " << sTile.index << std::endl;
  std::cout << "Starting Tile Position: ( " << sTile.x << ", " << sTile.y << std::endl;

  tileTransformWeak = tiles[sTile.index]->GetComponent<Transform>();
  tileTransform = tileTransformWeak.lock();

  playerTransform->position.x = tileTransform->position.x + centerOffset;
  playerTransform->position.y = tileTransform->position.y + centerOffset;

  int tileSize = gameMap.GetTileSize();

  Vector3 nextTile = sTile;
  Vector3 tryNextTile = nextTile;

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

    sf::Vector2f oldPosition({ playerTransform->position.x, playerTransform->position.y });

    //if (!isMoving)
    //{
    //  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    //  {
    //    direction = Direction::Left;
    //  }
    //  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    //  {
    //    direction = Direction::Right;
    //  }
    //  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    //  {
    //    direction = Direction::Up;
    //  }
    //  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    //  {
    //    direction = Direction::Down;
    //  }
    //}
    //else
    //{
    //  switch (direction)
    //  {
    //  case Direction::Left:
    //    playerTransform->position.x -= speed;
    //    break;
    //  case Direction::Right:
    //    playerTransform->position.x += speed;
    //    break;
    //  case Direction::Up:
    //    playerTransform->position.y -= speed;
    //    break;
    //  case Direction::Down:
    //    playerTransform->position.y += speed;
    //    break;
    //  default:
    //    break;
    //  }
    //}

    
    if (!isMoving)
    {
      tryNextTile = nextTile;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      {
        tryNextTile.x--;
        if (CheckNextTile(direction, tryNextTile, tiles, tileTransformWeak, tileTransform))
        {
          direction = Direction::Left;
        }
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      {
        tryNextTile.x++;
        if (CheckNextTile(direction, tryNextTile, tiles, tileTransformWeak, tileTransform))
        {
          direction = Direction::Right;
        }
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      {
        std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        tryNextTile.y--;
        if (CheckNextTile(direction, tryNextTile, tiles, tileTransformWeak, tileTransform))
        {
          direction = Direction::Up;
        }
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      {
        tryNextTile.y++;
        if (CheckNextTile(direction, tryNextTile, tiles, tileTransformWeak, tileTransform))
        {
          direction = Direction::Down;
        }
      }
      tryNextTile = nextTile;

      switch (direction)
      {
      case Direction::Left:
        tryNextTile.x--;
        break;
      case Direction::Right:
        tryNextTile.x++;
        break;
      case Direction::Up:
        tryNextTile.y--;
        break;
      case Direction::Down:
        tryNextTile.y++;
        break;
      default:
        break;
      }

      bool checkAdvance = CheckNextTile(direction, tryNextTile, tiles, tileTransformWeak, tileTransform);
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
          std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        }
        break;
      case Direction::Right:
        playerTransform->position.x += speed;
        if (static_cast<int>(playerTransform->position.x ) >= tileTransform->position.x + centerOffset)
        {
          playerTransform->position.x = tileTransform->position.x + centerOffset;
          isMoving = false;
          std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        }
        break;
      case Direction::Up:
        playerTransform->position.y -= speed;
        if (static_cast<int>(playerTransform->position.y) <= tileTransform->position.y + centerOffset)
        {
          playerTransform->position.y = tileTransform->position.y + centerOffset;
          isMoving = false;
          std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index: " << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        }
        break;
      case Direction::Down:
        playerTransform->position.y += speed;
        if (static_cast<int>(playerTransform->position.y) >= tileTransform->position.y + centerOffset)
        {
          playerTransform->position.y = tileTransform->position.y + centerOffset;
          isMoving = false;
          std::cout << "ReachedNextTile: Type: " << nextTile.type << "  Index:" << nextTile.index << "  Pos: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        }
        break;
      default:
        break;
      }
    }
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
    //{
    //  std::cout << "Current Position: " << playerTransform->position.x << ", " << playerTransform->position.y << std::endl;
    //}

    sf::FloatRect playerBounds = player->GetComponent<GraphicsComponent>().lock()->GetBounds();
    sf::FloatRect enemyBounds = enemy->GetComponent<GraphicsComponent>().lock()->GetBounds();

    if (const std::optional intersection = playerBounds.findIntersection(enemyBounds))
    {
      lives--;
      //respawn
      if (lives == 0)
      {
        //restart game
      }
    }
    UpdateScene(scene);
    gameMap.Render(window);
    RenderScene(scene, window);

    // Draw bounding boxes
    DrawBounds(window, playerBounds, sf::Color::Green);
    DrawBounds(window, enemyBounds, sf::Color::Red);

    //for (const auto& tile : tiles)
    //{
    //  sf::FloatRect bounds = tile->GetBounds();
    //  DrawBounds(window, bounds, sf::Color::Magenta);
    //}
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
bool CheckNextTile(Direction direction, Vector3& nextTile, const std::vector<std::shared_ptr<Tile>> tiles, std::weak_ptr<Transform>& tileTransformWeak,  std::shared_ptr<Transform>& tileTransform)
{
  //switch (direction)
  //{
  //case Direction::Left:
  //  nextTile.x--;
  //  break;
  //case Direction::Right:
  //  nextTile.x++;
  //  break;
  //case Direction::Up:
  //  nextTile.y--;
  //  break;
  //case Direction::Down:
  //  nextTile.y++;
  //  break;
  //default:
  //  break;
  //}
  nextTile.index = (nextTile.y * 28) + nextTile.x;

  tileTransformWeak = tiles[nextTile.index]->GetComponent<Transform>();
  tileTransform = tileTransformWeak.lock();

  nextTile = tiles[nextTile.index]->GetInfo();

  if (nextTile.index >= 0 && nextTile.index < static_cast<int>(tiles.size()) && nextTile.type != 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
