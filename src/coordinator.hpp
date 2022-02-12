#ifndef BLAST4_COORDINATOR_HPP
#define BLAST4_COORDINATOR_HPP

#include "board.hpp"
#include "context.hpp"
#include "settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

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
        void draw();

      private:
        sf::RenderWindow m_window;
        Settings m_settings;
        Board m_board;

        Context m_context;
    };

} // namespace blast4

#endif // BLAST4_COORDINATOR_HPP