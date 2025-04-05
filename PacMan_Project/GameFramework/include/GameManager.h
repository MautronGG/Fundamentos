#pragma once

enum class GameState { MainMenu, GameLoop, PauseMenu, GameOver, Exit, Restart, Save, Load };

class GameManager
{
public:
  static GameManager& instance()
  {
    static GameManager* instance = new GameManager();
    return *instance;
  }

  inline GameState GetGameState() const
  {
    return m_gameState;
  }

  void SetGameState(GameState state)
  {
    m_gameState = state;
  }
private:
  GameManager() {}
  GameState m_gameState = GameState::MainMenu;
};