#include "EntityManager.h"
#include "entity_manager.h"

EntityManager::EntityManager()
{
}

EntityManager *EntityManager::instance()
{
    static EntityManager i;
    return &i;
}

BaseGameEntity *EntityManager::getEntityFromId(int id) const
{
    auto ent = entityMap_.find(id);

    assert((ent != std::end(entityMap_)) && "<EntityManager::getEntityFromId>: invalid ID");

    return ent->second;
}

void EntityManager::removeEntity(BaseGameEntity *entity)
{
    entityMap_.erase(entityMap_.find(entity->ID()));
}

void EntityManager::registerEntity(BaseGameEntity *new_entity)
{
    entityMap_.insert(std::make_pair(new_entity->ID(), new_entity));
}