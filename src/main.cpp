// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stddef.h>

#include "coordinator.hpp"
#include "settings.hpp"

int main()
{
    using namespace blast4;

    Settings settings;

    Coordinator game(settings);
    game.play();

    return EXIT_SUCCESS;
}
