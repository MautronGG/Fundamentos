#pragma once

#include <string>
#include "Entity.h"

enum class UIType {Title, Button, Normal, None};
enum class ButtonType {Start, Continue, Exit, None};

class UIEntity : public Entity
{
public:
  UIEntity(ButtonType button, UIType type);

  void SetShape(int fontSize, std::string text, sf::Vector2f position, sf::Text textObject);
  void OnEnter();
  void UpdateText(int fontSize, std::string text, sf::Text textObject);
  inline const UIType GetType() const
  {
    return m_type;
  }

  UIType m_type = UIType::None;
  ButtonType m_buttonType = ButtonType::None;
  sf::RectangleShape m_shape;
  sf::Texture m_texture;
};