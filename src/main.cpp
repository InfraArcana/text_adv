#include "init.hpp"
#include "game.hpp"

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[])
{
    TRACE_FUNC_BEGIN;

    (void)argc;
    (void)argv;

    init::init_io();

    init::init_game();

    init::init_session();

    game::run();

    init::cleanup_session();

    init::cleanup_game();

    init::cleanup_io();

    TRACE_FUNC_END;
}
