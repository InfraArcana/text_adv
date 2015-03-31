#ifndef ACTOR_H
#define ACTOR_H

#include "cmn_types.hpp"

class Room;

class Actor
{
public:
    Actor(const Pos& p);

    void move(const Dir dir);

    Room* get_room();

    Pos get_pos() {return p_;}

protected:
    Pos p_;
};

#endif // ACTOR_H
