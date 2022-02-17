#ifndef BLAST4_COORDINATOR_HPP
#define BLAST4_COORDINATOR_HPP

#include "aliens.hpp"
#include "ammo.hpp"
#include "board.hpp"
#include "bullets.hpp"
#include "context.hpp"
#include "game.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
#include "states.hpp"
#include "top-panel.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace blast4
{

    class Coordinator
    {
      public:
        Coordinator(const Settings & settings);

      public:
        void play();

      private:
        void setup();
        void loop();
        void handleEvents();
        void handleEvent(const sf::Event & event);
        void update();
        void draw();

      private:
        util::Random m_random;
        sf::RenderWindow m_window;
        Settings m_settings;
        Game m_game;
        Board m_board;
        TopPanel m_panel;
        Starship m_starship;
        Bullets m_bullets;
        Aliens m_aliens;
        Ammo m_ammo;
        StateMachine m_states;
        util::SoundPlayer m_audio;

        Context m_context;
    };

} // namespace blast4

#endif // BLAST4_COORDINATOR_HPP
