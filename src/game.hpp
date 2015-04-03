#ifndef GAME_H
#define GAME_H

#include <string>

enum class Quit_Game {no, yes};

namespace game
{

extern Quit_Game quit_game;

void init();

void cleanup();

void run();

void add_msg(const std::string& msg);

} // game

#endif // GAME_H
