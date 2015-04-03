#include "game.hpp"

#include <algorithm>
#include <memory>

#include "init.hpp"
#include "io.hpp"
#include "parsers.hpp"
#include "map.hpp"
#include "utils.hpp"

namespace game
{

Quit_Game quit_game = Quit_Game::no;

namespace
{

std::string msg_ = "";

std::vector< std::unique_ptr<Cmd_Parser> > cmd_parsers;

void handle_cmd(const std::string& cmd_str)
{
    std::vector<std::string> tokenized_cmd = utils::split_str(cmd_str);

    const std::string first = tokenized_cmd[0];

    for (const auto& parser : cmd_parsers)
    {
        const std::vector<std::string> trigger_verbs = parser->get_trigger_verbs();

        if (std::find(begin(trigger_verbs), end(trigger_verbs), first) != end(trigger_verbs))
        {
            parser->handle_cmd(tokenized_cmd);
            break;
        }
    }

    if (msg_.empty())
    {
        add_msg("Sorry, that's not something I understand.");
    }
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    quit_game = Quit_Game::no;

    msg_ = "";

    // Set up parsers
    cmd_parsers.clear();
    cmd_parsers.push_back(std::unique_ptr<Cmd_Parser>(new Quit_Cmd_Parser));
    cmd_parsers.push_back(std::unique_ptr<Cmd_Parser>(new Go_Cmd_Parser));
    cmd_parsers.push_back(std::unique_ptr<Cmd_Parser>(new Look_Cmd_Parser));

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

void run()
{
    TRACE_FUNC_BEGIN;

    map::mk();

    std::string user_cmd_str = "";

    add_msg("Welcome to [TEXT ADVENTURE]!\n");

    add_msg(map::player->get_room()->get_descr());

    while (quit_game == Quit_Game::no)
    {
        // Read input, draw message history and draw the command being entered
        io::get_cmd(user_cmd_str, msg_);

        msg_ = "";

        // Handle the user command, and build a new message
        handle_cmd(user_cmd_str);
        assert(!msg_.empty());
    }

    TRACE_FUNC_END;
}

void add_msg(const std::string& msg)
{
    if (!msg.empty())
    {
        msg_ += "\n";
    }

    msg_ += msg;
}

} // game
