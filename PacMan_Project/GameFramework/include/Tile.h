#pragma once

#include "Entity.h"
#include "TransformComponent.h"

struct Vector3;

class Tile : public Entity
{
public:
  Tile(int type, float x, float y, int tileSize, const sf::Vector2f& offset, const int index);

  void Render(sf::RenderWindow& window);
  void SetShape();
  void SetType(int newType);

  inline const sf::RectangleShape& GetShape() const
  {
    return m_shape;
  }

  //Por ahora quité el & porque no lo necesitas
  inline const sf::FloatRect GetBounds() const
  {
    // getGlobalBounds() es una copia, sin emargo lo regresas como refrencia, lo que causa
    // que al salir de esta función en teoría se destruye el objeto
    // por lo que no puedes usarlo fuera de esta función.

    // Para solucionarlo puedes regresar una copia, sf::FloatRect sin el & o guardar una copia local y regresarla./
    return GetShape().getGlobalBounds();
  }
  inline const Vector3 GetInfo() const
  {
    return m_tileInfo;
  }

private:
  sf::RectangleShape m_shape;
  Vector3 m_tileInfo;
};