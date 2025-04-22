#pragma once

#include "UIEntity.h"

class MainMenu : public Entity
{
public:
  MainMenu() = default;
  ~MainMenu() = default;

  void MoveInMenu();

  void AddButton(std::shared_ptr<UIEntity>& button);
  void Update() override;

  const std::vector<std::shared_ptr<UIEntity>>& GetButtonArray() const
  {
    return m_buttonArray;
  }

  private:
  // Todo esto no debe ser público, para eso deberías tener un
  std::vector<std::shared_ptr<UIEntity>> m_buttonArray;
  int m_currentArrayPos = 0;
  bool m_sKeyPressed = false;
  bool m_wKeyPressed = false;
  bool m_enterKeyPressed = false;
};