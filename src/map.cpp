#include "map.hpp"

#include "init.hpp"

std::string Room::get_descr()
{
    return "You're in a room.";
}

namespace map
{

std::vector< std::unique_ptr<Actor> > actors;

Actor* player;

std::unique_ptr<Room> rooms[MAP_W][MAP_H];

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    actors.push_back(std::unique_ptr<Actor>(new Actor({0, 2})));

    player = actors[0].get();

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    actors.resize(0);

    for (int x = 0; x < MAP_W; ++x)
    {
        for (int y = 0; y < MAP_H; ++y)
        {
            rooms[x][y] = nullptr;
        }
    }

    TRACE_FUNC_END;
}

void mk()
{
    rooms[0][2] = std::unique_ptr<Room>(new Room());
    rooms[1][2] = std::unique_ptr<Room>(new Room());
    rooms[0][1] = std::unique_ptr<Room>(new Room());
}

} // map
