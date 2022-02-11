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
