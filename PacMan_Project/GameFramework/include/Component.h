#pragma once

class Entity;

class Component
{
public:
  Component() = default;
  virtual ~Component() = default;

  virtual void Update(const Entity* parent/*DeltaTime*/) = 0;
};