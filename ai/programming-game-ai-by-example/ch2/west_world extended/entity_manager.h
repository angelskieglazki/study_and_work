#pragma once

class EntityManager {
private:
  using EntityMap = std::map<int, BaseGameEntity*>;
  EntityMap entityMap_;

private:
  EntityManager() = delete;
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

public:
  ~EntityManager() = default;

  static* EntityManager* instance();


};
