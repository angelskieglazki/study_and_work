#include "base_game_entity.h"
#include <cassert>

int BaseGameEntity::next_valid_id_ = 0;

int BaseGameEntity::getId() const noexcept
{
    return id_;
}

void BaseGameEntity::setId(int id)
{
    assert((id >= next_valid_id_) && "<BaseGameEntity::setId>: invalid ID");

    id_ = id;

    next_valid_id_ = id + 1;
}