#include "parsers.hpp"

#include "game.hpp"
#include "map.hpp"
#include "utils.hpp"

//----------------------------------------------------------------------------- QUIT COMMAND PARSER
std::vector<std::string> Quit_Cmd_Parser::get_trigger_verbs() const
{
    return {"exit", "quit", "q"};
}

void Quit_Cmd_Parser::handle_cmd(std::vector<std::string>& cmd) const
{
    (void)cmd;

    if (cmd.size() == 1)
    {
        game::quit_game = Quit_Game::yes;
    }
}

//----------------------------------------------------------------------------- GO COMMAND PARSER
std::vector<std::string> Go_Cmd_Parser::get_trigger_verbs() const
{
    return {"go", "walk"};
}

void Go_Cmd_Parser::handle_cmd(std::vector<std::string>& cmd) const
{
    Dir dir = Dir::END;

    if (cmd.size() == 1)
    {
        game::add_msg("Go where?");
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
                game::add_msg(map::player->get_room()->get_descr());
            }
            else
            {
                game::add_msg("You see no passage that way.");
            }
        }
    }
}

//----------------------------------------------------------------------------- LOOK COMMAND PARSER
std::vector<std::string> Look_Cmd_Parser::get_trigger_verbs() const
{
    return {"look"};
}

void Look_Cmd_Parser::handle_cmd(std::vector<std::string>& cmd) const
{
    if (cmd.size() == 1)
    {
        game::add_msg(map::player->get_room()->get_descr());
    }
}
