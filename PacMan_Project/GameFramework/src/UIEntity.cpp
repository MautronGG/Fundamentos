#include "UIEntity.h"
#include "TransformComponent.h"

void UIEntity::SetShape(UIType type, int fontSize, std::string text, sf::Vector2f position, sf::Text textObject)
{
  m_type = type;
  sf::Vector2f size(0.0f, 0.0f);
  switch (type)
  {
  case UIType::Button:
    m_shape.setFillColor(sf::Color(170, 192, 194));
    //m_shape.setOutlineColor(sf::Color::Black);
    size = { 200, 200 };
    break;
  case UIType::Title:
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Black);
    size = { 600, 600 };
  }

  m_shape.setSize(size);
  m_shape.setOrigin(m_shape.getGlobalBounds().getCenter());

  m_transformWeak = GetComponent<Transform>();
  m_transform = m_transformWeak.lock();
  m_transform->position.x = position.x;
  m_transform->position.y = position.y;
  m_shape.setPosition(position);

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