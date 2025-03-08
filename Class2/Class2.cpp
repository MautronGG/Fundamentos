
#include "Class2.h"
#include <random>
#include <array>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

void printEntities(const vector<Entity*>& entities)
{
  cout << endl;
  cout << "Printing Entitites: " << endl;
  for (const auto& entity : entities)
  {
    entity->Draw();
  }
}
struct Enemy
{
public:
	int m_health;
  float m_x;
  float m_y;
};

int main()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> startingRange(50, 100);
  uniform_int_distribution<> healthRange(0, 10);
  uniform_int_distribution<> randomRange(-10, 10);

  array<Enemy, 5> enemiesArr;

  Enemy* enemyPtr = enemiesArr.data();

  for (int i = 0; i < 5; i++, enemyPtr++)
  {
    enemyPtr->m_health = startingRange(gen);
    enemyPtr->m_x = startingRange(gen);
    enemyPtr->m_y = startingRange(gen);
  }

  enemyPtr = enemiesArr.data();
  for (int i = 0; i < 5; i++, enemyPtr++)
  {
    cout << "Enemy " << i+1 << " Health: " << enemyPtr->m_health << ", Position : [" << enemyPtr->m_x << ", " << enemyPtr->m_y << "] " << endl;
  }
  cout << endl;
  enemyPtr = enemiesArr.data();
  for (int i = 0; i < 5; i++, enemyPtr++)
  {
    enemyPtr->m_health -= healthRange(gen);
    enemyPtr->m_x += randomRange(gen);
    enemyPtr->m_y += randomRange(gen);
  }

  enemyPtr = enemiesArr.data();
  for (int i = 0; i < 5; i++, enemyPtr++)
  {
    cout << "New Enemy " << i+1 << " Health: " << enemyPtr->m_health << ", Position: [ " << enemyPtr->m_x << ", " << enemyPtr->m_y << "] " << endl;
  }	

  Circle c(10, 20, 5);
  Rectangle r(30, 40, 10, 20);
  Sprite s(50, 60, "sprite.png");
  Text t(70, 80, "Hello World");

  // Store pointers to entities in a vector
  vector<Entity*> entities = { &c, &r, &s, &t };

  // Call the printEntities function to print the entities
  printEntities(entities);

  return 0;
}
