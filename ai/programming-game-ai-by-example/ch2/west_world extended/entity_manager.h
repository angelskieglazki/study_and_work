#pragma once

#include <map>

class BaseGameEntity;

#define EntityMgr EntityManager::instance()
class EntityManager {
private:
  using EntityMap = std::map<int, BaseGameEntity*>;
  EntityMap entityMap_;

private:
  EntityManager();
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

public:
  ~EntityManager() = default;

  static EntityManager* instance();

  void registerEntity(BaseGameEntity* new_entity);
  BaseGameEntity* getEntityFromId(int id) const;
  void removeEntity(BaseGameEntity* entity);
};
