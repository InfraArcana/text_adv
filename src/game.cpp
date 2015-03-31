#include "game.hpp"

#include <string>

#include "init.hpp"
#include "io.hpp"
#include "map.hpp"
#include "utils.hpp"

enum class Quit_game {no, yes};

namespace game
{

namespace
{

std::string msg_ = "";

void add_msg(const std::string& msg)
{
    if (!msg.empty())
    {
        msg_ += "\n";
    }

    msg_ += msg;
}

Quit_game handle_cmd(const std::string& cmd)
{
    if (cmd == "exit" || cmd == "quit" || cmd == "q")
    {
        add_msg("Bye!");
        return Quit_game::yes;
    }

    if (cmd == "look")
    {
        add_msg(map::player->get_room()->get_descr());
        return Quit_game::no;
    }

    // Travel compass direction?
    Dir dir = Dir::END;

    if (cmd == "go east")
    {
        dir = Dir::right;
    }
    else if (cmd == "go north")
    {
        dir = Dir::up;
    }
    else if (cmd == "go west")
    {
        dir = Dir::left;
    }
    else if (cmd == "go south")
    {
        dir = Dir::down;
    }

    if (dir != Dir::END)
    {
        const Pos& player_pos = map::player->get_pos();

        const Pos new_pos = player_pos + dir_utils::get_offset(dir);

        Room* new_room = utils::is_in_map(new_pos) ? map::rooms[new_pos.x][new_pos.y].get() : nullptr;

        if (new_room)
        {
            map::player->move(dir);
            add_msg(map::player->get_room()->get_descr());
            return Quit_game::no;
        }
        else
        {
            add_msg("You see no passage that way.");
            return Quit_game::no;
        }
    }

    add_msg("That's not something I understand.");

    return Quit_game::no;
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    msg_ = "";

    TRACE_FUNC_END;
}

void run()
{
    TRACE_FUNC_BEGIN;

    map::mk();

    std::string user_cmd = "";

    add_msg("Welcome to [TEXT ADVENTURE]!");

    add_msg(map::player->get_room()->get_descr());

    Quit_game quit_game = Quit_game::no;

    //-----------------------------
    //
    //-----------------------------

    while (quit_game == Quit_game::no)
    {
        // Read input, draw message history and draw the command being entered
        io::get_cmd(user_cmd, msg_);

        msg_ = "";

        // Handle the user command, and build a new message
        quit_game = handle_cmd(user_cmd);
        assert(!msg_.empty());
    }

    TRACE_FUNC_END;
}

} // game
