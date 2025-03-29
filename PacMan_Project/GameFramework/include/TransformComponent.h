#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

struct Vector2
{
  float x = 0.0f;
  float y = 0.0f;
};

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