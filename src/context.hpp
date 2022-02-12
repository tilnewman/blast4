#ifndef BLAST4_CONTEXT_HPP
#define BLAST4_CONTEXT_HPP

// Note:  Be careful to only keep references in the Context class and/so no includes in this file.

namespace sf
{
    class RenderWindow;
}

namespace blast4
{

    class Board;
    class Images;
    struct Settings;

    struct Context
    {
        Context(sf::RenderWindow & win, const Settings & set, Board & boa, Images & ima)
            : window(win)
            , settings(set)
            , board(boa)
            , images(ima)
        {}

        sf::RenderWindow & window;
        const Settings & settings;
        Board & board;
        Images & images;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
