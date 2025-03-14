// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "coordinator.hpp"

#include "check-macros.hpp"
#include "sfml-defaults.hpp"

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
        , m_ammo()
        , m_states()
        , m_audio(m_random)
        , m_anim(m_random, (settings.media_path / "animation").string())
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
              m_ammo,
              m_states,
              m_audio,
              m_anim)
    {}

    void Coordinator::play()
    {
        setup();
        loop();
        teardown();
    }

    void Coordinator::setup()
    {
        util::SfmlDefaults::instance().setup();

        m_audio.mediaPath(m_settings.media_path / "sound");
        m_audio.loadAll();

        m_anim.loadAll();

        m_window.create(sf::VideoMode::getDesktopMode(), "Blast4", sf::State::Fullscreen);
        M_CHECK(m_window.isOpen(), "Error:  Failed to open graphics window.");

        m_window.setFramerateLimit(60);
        m_window.setKeyRepeatEnabled(false);
        m_window.setMouseCursorVisible(false);

        m_board.setup(m_context);
        m_panel.setup(m_context);
        m_starship.setup(m_context);
        m_bullets.setup(m_context);
        m_aliens.setup(m_context);
        m_ammo.setup(m_context);

        m_game.ammo = m_settings.starting_ammo;
        m_states.setChangePending(State::Start);
    }

    void Coordinator::teardown() { util::SfmlDefaults::instance().teardown(); }

    void Coordinator::loop()
    {
        sf::Clock frameClock;

        while (m_window.isOpen() && (m_states.which() != State::Teardown))
        {
            m_context.frame_time_sec = frameClock.restart().asSeconds();

            m_states.changeIfPending(m_context);

            m_scoring.preTurn(m_context);
            handleEvents();
            update();
            m_scoring.postTurn(m_context);
            draw();
        }

        std::cout << "Final Score: " << m_game.score << std::endl;
    }

    void Coordinator::handleEvents()
    {
        while (const auto eventOpt = m_window.pollEvent())
        {
            handleEvent(eventOpt.value());
        }
    }

    void Coordinator::handleEvent(const sf::Event & event)
    {
        m_states.state().handleEvent(m_context, event);
    }

    void Coordinator::update() { m_states.state().update(m_context); }

    void Coordinator::draw()
    {
        m_window.clear(m_context.settings.background_color);
        m_states.state().draw(m_context);
        m_window.display();
    }

} // namespace blast4
