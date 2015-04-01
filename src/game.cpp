#include "game.hpp"

#include <string>
#include <vector>

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

void handle_look_cmd(const std::vector<std::string>& cmd)
{
    if (cmd.size() == 1)
    {
        add_msg(map::player->get_room()->get_descr());
    }
}

void handle_go_cmd(const std::vector<std::string>& cmd)
{
    Dir dir = Dir::END;

    if (cmd.size() == 1)
    {
        add_msg("Go where?");
    }
    else if (cmd.size() == 2)
    {
        const std::string second = cmd[1];

        if (second == "east")
        {
            dir = Dir::right;
        }
        else if (second == "north")
        {
            dir = Dir::up;
        }
        else if (second == "west")
        {
            dir = Dir::left;
        }
        else if (second == "south")
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
            }
            else
            {
                add_msg("You see no passage that way.");
            }
        }
    }
}

Quit_game handle_cmd(const std::string& cmd_str)
{
    std::vector<std::string> cmd = utils::split_str(cmd_str);

    const std::string first = cmd[0];

    if (cmd.size() == 1 && (first == "exit" || first == "quit" || first == "q"))
    {
        add_msg("Bye!");
        return Quit_game::yes;
    }
    else if (first == "look")
    {
        handle_look_cmd(cmd);
    }
    else if (first == "go" || first == "walk")
    {
        handle_go_cmd(cmd);
    }

    if (msg_.empty())
    {
        add_msg("That's not something I understand.");
    }

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

    std::string user_cmd_str = "";

    add_msg("Welcome to [TEXT ADVENTURE]!\n");

    add_msg(map::player->get_room()->get_descr());

    Quit_game quit_game = Quit_game::no;

    //-----------------------------
    //
    //-----------------------------

    while (quit_game == Quit_game::no)
    {
        // Read input, draw message history and draw the command being entered
        io::get_cmd(user_cmd_str, msg_);

        msg_ = "";

        // Handle the user command, and build a new message
        quit_game = handle_cmd(user_cmd_str);
        assert(!msg_.empty());
    }

    TRACE_FUNC_END;
}

} // game
