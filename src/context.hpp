#ifndef BLAST4_CONTEXT_HPP
#define BLAST4_CONTEXT_HPP

// Note:  Be careful to only keep references in the Context class and/so no includes in this file.

namespace sf
{
    class RenderWindow;
}

namespace blast4
{

    struct Settings;
    class Board;

    struct Context
    {
        Context(sf::RenderWindow & win, const Settings & set, Board & boa)
            : window(win)
            , settings(set)
            , board(boa)
        {}

        sf::RenderWindow & window;
        const Settings & settings;
        Board & board;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
