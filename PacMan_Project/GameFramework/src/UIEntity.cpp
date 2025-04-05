#include "UIEntity.h"
#include "TransformComponent.h"
#include "GameManager.h"

UIEntity::UIEntity(ButtonType button, UIType type) : m_buttonType(button), m_type(type){}

void UIEntity::SetShape(int fontSize, std::string text, sf::Vector2f position, sf::Text textObject)
{
  sf::Vector2f size(0.0f, 0.0f);
  switch (m_type)
  {
  case UIType::Button:
    m_shape.setFillColor(sf::Color(170, 192, 194));
    //m_shape.setOutlineColor(sf::Color::Black);
    size = { 400, 200 };
    break;
  case UIType::Title:
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOutlineColor(sf::Color::Black);
    size = { 600, 400 };
    break;
  case UIType::Normal:
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Black);
    size = { 600, 200 };
    break;
  }

  m_shape.setSize(size);
  m_shape.setOrigin(m_shape.getGlobalBounds().getCenter());

  m_transformWeak = GetComponent<Transform>();
  m_transform = m_transformWeak.lock();
  m_transform->position.x = position.x;
  m_transform->position.y = position.y;
  m_shape.setPosition({size.x/2, size.y / 2 });

  textObject.setString(text);
  textObject.setCharacterSize(fontSize);
  textObject.setFillColor(sf::Color::Black);
  textObject.setOrigin(textObject.getGlobalBounds().getCenter());
  textObject.setPosition(m_shape.getGlobalBounds().getCenter());

  // Create a RenderTexture
  sf::RenderTexture renderTexture({ static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) });

  // Clear the RenderTexture
  renderTexture.clear(sf::Color::Transparent);

  // Draw the shape and text onto the RenderTexture
  renderTexture.draw(m_shape);
  renderTexture.draw(textObject);

  // Display the RenderTexture
  renderTexture.display();

  // Retrieve the texture from the RenderTexture
  m_texture = renderTexture.getTexture();
}
void UIEntity::UpdateText(int fontSize, std::string text, sf::Text textObject)
{
  sf::Vector2f size(0.0f, 0.0f);
  switch (m_type)
  {
  case UIType::Button:
    m_shape.setFillColor(sf::Color(170, 192, 194));
    //m_shape.setOutlineColor(sf::Color::Black);
    size = { 400, 200 };
    break;
  case UIType::Title:
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOutlineColor(sf::Color::Black);
    size = { 600, 400 };
    break;
  case UIType::Normal:
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Black);
    size = { 600, 200 };
    break;
  }

  textObject.setString(text);
  textObject.setCharacterSize(fontSize);
  textObject.setFillColor(sf::Color::Black);
  textObject.setOrigin(textObject.getGlobalBounds().getCenter());
  textObject.setPosition(m_shape.getGlobalBounds().getCenter());

  // Create a RenderTexture
  sf::RenderTexture renderTexture({ static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) });

  // Clear the RenderTexture
  renderTexture.clear(sf::Color::Transparent);

  // Draw the shape and text onto the RenderTexture
  renderTexture.draw(m_shape);
  renderTexture.draw(textObject);

  // Display the RenderTexture
  renderTexture.display();

  // Retrieve the texture from the RenderTexture
  m_texture = renderTexture.getTexture();
}
void UIEntity::OnEnter()
{
  GameManager& manager = GameManager::instance();

  switch (m_buttonType)
  {
  case ButtonType::Start:
    switch (manager.GetGameState())
    {
    case GameState::MainMenu:
      manager.SetGameState(GameState::Restart);
      break;
    case GameState::PauseMenu:
      manager.SetGameState(GameState::GameLoop);
      break;
    case GameState::GameOver:
      manager.SetGameState(GameState::GameLoop);
      break;
    default:
      break;
    }
    break;
  case ButtonType::Continue:
    switch (manager.GetGameState())
    {
    case GameState::MainMenu:
      manager.SetGameState(GameState::Load);
      break;
    case GameState::PauseMenu:
      manager.SetGameState(GameState::Restart);
      break;
    case GameState::GameOver:
      manager.SetGameState(GameState::Restart);
      break;
    default:
      break;
    }
    break;
  case ButtonType::Exit:
    switch (manager.GetGameState())
    {
    case GameState::MainMenu:
      manager.SetGameState(GameState::Exit);
      break;
    case GameState::PauseMenu:
      manager.SetGameState(GameState::Save);
      break;
    case GameState::GameOver:
      manager.SetGameState(GameState::MainMenu);
      break;
    default:
      break;
    }
    break;
  }
}