#pragma once

#include "Component.h"

typedef void (*ComponentScript)(const Entity* parent);

class ScriptComponent : public Component
{
public:
  ScriptComponent(ComponentScript scriptFunction)
    : m_scriptFunction(scriptFunction) {
  };
  ~ScriptComponent() = default;

  void Update(const Entity* parent/*DeltaTime*/) override;

private:
  //Function pointer
  ComponentScript m_scriptFunction = nullptr;

};