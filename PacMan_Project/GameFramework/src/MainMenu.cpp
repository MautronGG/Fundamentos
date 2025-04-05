#include "MainMenu.h"
#include "GraphicsComponent.h"

void MainMenu::MoveInMenu()
{
  if (m_buttonArray[m_currentArrayPos]->GetType() != UIType::Button)
  {
    m_currentArrayPos++;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
  {
    if (!m_sKeyPressed)
    {
      m_sKeyPressed = true;
      m_currentArrayPos++;
      if (m_currentArrayPos >= m_buttonArray.size())
      {
        m_currentArrayPos = 0;
      }
      while (m_buttonArray[m_currentArrayPos]->GetType() != UIType::Button)
      {
        m_currentArrayPos++;
        if (m_currentArrayPos >= m_buttonArray.size())
        {
          m_currentArrayPos = 0;
        }
      }
    }
  }
  else
  {
    m_sKeyPressed = false;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
  {
    if (!m_wKeyPressed)
    {
      m_wKeyPressed = true;
      m_currentArrayPos--;
      if (m_currentArrayPos < 0)
      {
        m_currentArrayPos = m_buttonArray.size() - 1;
      }
      while (m_buttonArray[m_currentArrayPos]->GetType() != UIType::Button)
      {
        m_currentArrayPos--;
        if (m_currentArrayPos < 0)
        {
          m_currentArrayPos = m_buttonArray.size() - 1;
        }
      }
    }
  }
  else
  {
    m_wKeyPressed = false;
  }

  for (int i = 0; i < m_buttonArray.size(); i++)
  {
    if (i != m_currentArrayPos)
    {
      m_buttonArray[i]->GetComponent<GraphicsComponent>().lock()->SetSpriteColor(sf::Color::White);
    }
  }
  m_buttonArray[m_currentArrayPos]->GetComponent<GraphicsComponent>().lock()->SetSpriteColor(sf::Color::Yellow);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
  {
    if (!m_enterKeyPressed) // Check if the Enter key was not previously pressed
    {
      m_enterKeyPressed = true; // Set the flag to true
      m_buttonArray[m_currentArrayPos]->OnEnter();
    }
  }
  else
  {
    m_enterKeyPressed = false; // Reset the flag when the Enter key is released
  }
}

void MainMenu::Update()
{
  Entity::Update();
  MoveInMenu();
}

void MainMenu::AddButton(std::shared_ptr<UIEntity>& button)
{
  m_buttonArray.push_back(button);
}
