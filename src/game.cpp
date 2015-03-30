#include "game.hpp"

#include <string>

#include "init.hpp"
#include "io.hpp"

namespace game
{

void run()
{
    TRACE_FUNC_BEGIN;

    std::string msg = "Hello and welcome!\n\nSo what do you think of this string?\nCool huh?";
    std::string cmd = "";

    while (true)
    {
        io::get_cmd(cmd, msg);

        if (cmd == "exit" || cmd == "quit" || cmd == "q")
        {
            break;
        }

        msg = "Derp";
    }

    TRACE_FUNC_END;
}

}
