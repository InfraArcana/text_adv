#include "actor.hpp"

#include <assert.h>

#include "map.hpp"
#include "utils.hpp"

Actor::Actor(const Pos& p) :
    p_(p) {}

void Actor::move(const Dir dir)
{
    assert(dir != Dir::END && dir != Dir::center);

    const Pos d = dir_utils::get_offset(dir);

    assert(dir_utils::is_cardinal(d));

    p_ += d;

    assert(utils::is_in_map(p_));
}

Room* Actor::get_room()
{
    return map::rooms[p_.x][p_.y].get();
}
