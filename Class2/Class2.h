#ifndef CLASS2_H
#define CLASS2_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Entity
{
private:
  float m_x;
  float m_y;

public:
  Entity(float x = 0, float y = 0) : m_x(x), m_y(y) {}

  float getX() const
  {
    return m_x;
  }

  float getY() const
  {
    return m_y;
  }

  void setX(float x)
  {
    m_x = x;
  }

  void setY(float y)
  {
    m_y = y;
  }

  void Move(float x, float y)
  {
    m_x += x;
    m_y += y;
  }

  virtual void Draw() const = 0;
};

class Figure : public Entity
{
public:
  Figure(float x = 0, float y = 0) : Entity(x, y) {}

  virtual float getArea() const = 0;
};

class Circle : public Figure
{
private:
  float m_radius;

public:
  Circle(float x, float y, float radius) : Figure(x, y), m_radius(radius) {}

  void Draw() const override
  {
    cout << "Drawing a circle at (" << getX() << ", " << getY() << ") with radius " << m_radius << endl;
  }

  float getArea() const override
  {
    return 3.14f * m_radius * m_radius;
  }
};

class Rectangle : public Figure
{
private:
  float m_width;
  float m_height;

public:
  Rectangle(float x, float y, float width, float height) : Figure(x, y), m_width(width), m_height(height) {}

  void Draw() const override
  {
    cout << "Drawing a rectangle at (" << getX() << ", " << getY() << ") with width " << m_width << " and height " << m_height << endl;
  }

  float getArea() const override
  {
    return m_width * m_height;
  }
};

class Sprite : public Entity
{
private:
  string m_fileName;

public:
  Sprite(float x, float y, const string& fileName) : Entity(x, y), m_fileName(fileName) {}

  void Draw() const override
  {
    cout << "Drawing a sprite at (" << getX() << ", " << getY() << ") from file " << m_fileName << endl;
  }
};

class Text : public Entity
{
private:
  string m_content;

public:
  Text(float x, float y, const string& content) : Entity(x, y), m_content(content) {}

  void Draw() const override
  {
    cout << "Drawing text at (" << getX() << ", " << getY() << "): " << m_content << endl;
  }
};

//int printEntities()
//{
//  cout << "Printing entities" << endl;
//  Circle c(10, 20, 5);
//  Rectangle r(30, 40, 10, 20);
//  Sprite s(50, 60, "sprite.png");
//  Text t(70, 80, "Hello World");
//
//  c.Draw();
//  r.Draw();
//  s.Draw();
//  t.Draw();
//
//  return 0;
//}
void printEntities(const vector<Entity*>& entities);

#endif // CLASS2_H