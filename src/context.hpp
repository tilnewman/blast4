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
    class Bullets;
    struct Settings;

    struct Context
    {
        Context(
            sf::RenderWindow & win, const Settings & set, Board & boa, Images & ima, Bullets & bul)
            : window(win)
            , settings(set)
            , board(boa)
            , images(ima)
            , bullets(bul)
            , frame_time_sec(0.0f)
        {}

        sf::RenderWindow & window;
        const Settings & settings;
        Board & board;
        Images & images;
        Bullets & bullets;

        float frame_time_sec;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
