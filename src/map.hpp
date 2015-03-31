#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <memory>
#include <vector>

#include "cmn_data.hpp"
#include "actor.hpp"

class Room
{
public:
    Room() {}

    std::string get_descr();

private:
};

namespace map
{

extern std::unique_ptr<Room> rooms[MAP_W][MAP_H];

extern std::vector< std::unique_ptr<Actor> > actors;

extern Actor* player;

void init();

void cleanup();

void mk();

} // map

#endif // GAME_MAP_H
