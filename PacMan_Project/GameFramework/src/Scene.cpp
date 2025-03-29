#include "Scene.h"
#include "Entity.h"

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
  m_entities.push_back(entity);
}