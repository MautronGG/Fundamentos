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

  std::vector<std::shared_ptr<UIEntity>> m_buttonArray;
  int m_currentArrayPos = 0;
  bool m_sKeyPressed = false;
  bool m_wKeyPressed = false;
  bool m_enterKeyPressed = false;
};