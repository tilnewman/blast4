#include "coordinator.hpp"

#include "check-macros.hpp"
#include "util.hpp"

namespace blast4
{

    Coordinator::Coordinator(const Settings & settings)
        : m_random()
        , m_window()
        , m_settings(settings)
        , m_game()
        , m_board()
        , m_panel()
        , m_starship()
        , m_bullets()
        , m_aliens()
        , m_audio(m_random)
        , m_context(
              m_random,
              m_window,
              m_settings,
              m_game,
              m_board,
              m_panel,
              m_starship,
              m_bullets,
              m_aliens,
              m_audio)
    {}

    void Coordinator::play()
    {
        setup();
        loop();
    }

    void Coordinator::setup()
    {
        m_audio.mediaPath("media/sound");
        m_audio.loadAll();

        m_window.create(sf::VideoMode::getDesktopMode(), "Blast4", sf::Style::Fullscreen);
        M_CHECK(m_window.isOpen(), "Error:  Failed to open graphics window!");

        // m_window.setFramerateLimit(60);
        m_window.setKeyRepeatEnabled(false);
        m_window.setMouseCursorVisible(false);

        m_board.setup(m_context);
        m_panel.setup(m_context);
        m_starship.setup(m_context);
        m_bullets.setup(m_context);
        m_aliens.setup(m_context);

        m_game.ammo = m_settings.starting_ammo;
    }

    void Coordinator::loop()
    {
        sf::Clock frameClock;

        while (m_window.isOpen())
        {
            m_context.frame_time_sec = frameClock.restart().asSeconds();
            handleEvents();
            update();
            draw();
        }
    }

    void Coordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void Coordinator::handleEvent(const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            return;
        }

        m_starship.handleEvent(m_context, event);

        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
            return;
        }
    }

    void Coordinator::update()
    {
        m_starship.update(m_context);
        m_aliens.update(m_context);
        m_bullets.update(m_context);
        m_panel.update(m_context);
    }

    void Coordinator::draw()
    {
        m_window.clear(m_context.settings.background_color);

        m_board.draw(m_context);
        m_panel.draw(m_context);
        m_aliens.draw(m_context);
        m_starship.draw(m_context);
        m_bullets.draw(m_context);

        m_window.display();
    }

} // namespace blast4
