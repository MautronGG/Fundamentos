#pragma once

#include "UIEntity.h"

class MainMenu
{
public:
  MainMenu() = default;
  ~MainMenu() = default;

  void MoveInMenu();

  void AddButton(std::shared_ptr<UIEntity>& button);

  std::vector<std::shared_ptr<UIEntity>> m_buttonArray;
  int m_currentArrayPos = 0;
};