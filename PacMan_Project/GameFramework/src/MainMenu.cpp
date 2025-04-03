#include "MainMenu.h"

void MainMenu::MoveInMenu()
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
  {
    m_currentArrayPos++;
    while (m_buttonArray[m_currentArrayPos]->GetType() != UIType::Button)
    {
      m_currentArrayPos++;
      if (m_currentArrayPos >= m_buttonArray.size())
      {
        m_currentArrayPos = 0;
      }
    }
   
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
  {
    m_currentArrayPos--;
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

void MainMenu::AddButton(std::shared_ptr<UIEntity>& button)
{
  m_buttonArray.push_back(button);
}
