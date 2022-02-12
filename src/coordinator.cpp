#include "coordinator.hpp"

#include "check-macros.hpp"

#include <SFML/Window/event.hpp>

namespace blast4
{

    Coordinator::Coordinator(const Settings & settings)
        : m_window()
        , m_settings(settings)
        , m_board()
        , m_context(m_window, m_settings, m_board)
    {}

    void Coordinator::play()
    {
        setup();
        loop();
    }

    void Coordinator::setup()
    {
        m_window.create(sf::VideoMode::getDesktopMode(), "Blast4", sf::Style::Fullscreen);
        M_CHECK(m_window.isOpen(), "Error:  Failed to open graphics window!");

        m_board.setup(m_context);
    }

    void Coordinator::loop()
    {
        while (m_window.isOpen())
        {
            handleEvents();
            draw();
        }
    }

    void Coordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                m_window.close();
            }
        }
    }

    void Coordinator::draw()
    {
        m_window.clear(m_context.settings.background_color);
        m_board.draw(m_context);
        m_window.display();
    }

} // namespace blast4
