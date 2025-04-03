#pragma once

#include "Vector2.h"
#include "Component.h"

class Transform : public Component
{
public:
  Transform() = default;
  ~Transform() = default;

  void Update(const Entity* parent/*DeltaTime*/) override;

  Vector2 position;
  Vector2 scale;
  float rotation = 0.0f;
};