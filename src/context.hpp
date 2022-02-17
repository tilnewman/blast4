#ifndef BLAST4_CONTEXT_HPP
#define BLAST4_CONTEXT_HPP

// Note:  Be careful to only keep references in the Context class and/so no includes in this file.

namespace sf
{
    class RenderWindow;
} // namespace sf

namespace util
{
    class Random;
    class SoundPlayer;
} // namespace util

namespace blast4
{
    class Ammo;
    class Board;
    class Aliens;
    class Bullets;
    class Starship;
    class TopPanel;
    class StateMachine;

    struct Game;
    struct Settings;

    struct Context
    {
        Context(
            util::Random & ran,
            sf::RenderWindow & win,
            const Settings & set,
            Game & gam,
            Board & boa,
            TopPanel & top,
            Starship & sta,
            Bullets & bul,
            Aliens & ali,
            Ammo & amm,
            StateMachine & sm,
            util::SoundPlayer & sou)
            : random(ran)
            , window(win)
            , settings(set)
            , game(gam)
            , board(boa)
            , panel(top)
            , starship(sta)
            , bullets(bul)
            , aliens(ali)
            , ammo(amm)
            , states(sm)
            , audio(sou)
            , frame_time_sec(0.0f)
        {}

        util::Random & random;
        sf::RenderWindow & window;
        const Settings & settings;
        Game & game;
        Board & board;
        TopPanel & panel;
        Starship & starship;
        Bullets & bullets;
        Aliens & aliens;
        Ammo & ammo;
        StateMachine & states;
        util::SoundPlayer & audio;

        float frame_time_sec;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
