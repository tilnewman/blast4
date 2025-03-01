// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "states.hpp"

#include "aliens.hpp"
#include "ammo.hpp"
#include "animation-player.hpp"
#include "board.hpp"
#include "bullets.hpp"
#include "check-macros.hpp"
#include "game.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
#include "top-panel.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    void StateBase::update(Context & t_context) { t_context.panel.update(t_context); }

    void StateBase::draw(Context & t_context)
    {
        t_context.board.draw(t_context);
        t_context.panel.draw(t_context);
        t_context.ammo.draw(t_context);
        t_context.aliens.draw(t_context);
        t_context.starship.draw(t_context);
        t_context.bullets.draw(t_context);
        t_context.anim.draw(t_context.window, {});
    }

    void StateBase::handleCloseEvents(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.is<sf::Event::Closed>())
        {
            t_context.window.close();
        }
        else if (const auto * keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            if (sf::Keyboard::Scancode::Escape == keyPtr->scancode)
            {
                t_context.window.close();
            }
        }
    }

    void StateBase::setupTextMessage(
        Context & t_context, const std::string & t_message, sf::Text & t_text)
    {
        t_text.setFont(t_context.panel.titleFont());
        t_text.setFillColor(t_context.settings.title_color);
        t_text.setCharacterSize(200);
        t_text.setString(t_message);
        util::setOriginToPosition(t_text);

        const sf::FloatRect boardRect{ t_context.board.rect() };
        const float height{ boardRect.size.y * 0.2f };
        const sf::FloatRect fitRect{ { 0.0f, 0.0f }, { boardRect.size.x, height } };
        util::fit(t_text, fitRect);
        t_text.setPosition(util::center(boardRect) - (util::size(t_text) * 0.5f));
    }

    void StartState::OnEnter(Context & t_context)
    {
        m_timerSec = 5.0f;
        t_context.audio.play("game-start");
    }

    void StartState::update(Context & t_context)
    {
        m_timerSec -= t_context.frame_time_sec;
        if (m_timerSec < 0.0f)
        {
            t_context.states.setChangePending(State::Play);
        }
    }

    void EndState::OnEnter(Context & t_context)
    {
        m_timerSec = 5.0f;
        t_context.audio.play("game-end");
    }

    void EndState::update(Context & t_context)
    {
        t_context.anim.update(t_context.frame_time_sec);

        m_timerSec -= t_context.frame_time_sec;
        if (m_timerSec < 0.0f)
        {
            t_context.states.setChangePending(State::Teardown);
        }
    }

    void EndState::draw(Context & t_context)
    {
        t_context.board.draw(t_context);
        t_context.panel.draw(t_context);
        t_context.ammo.draw(t_context);
        t_context.aliens.draw(t_context);
        t_context.anim.draw(t_context.window, {});
    }

    void PlayState::update(Context & t_context)
    {
        StateBase::update(t_context);
        t_context.starship.update(t_context);
        t_context.aliens.update(t_context);
        t_context.bullets.update(t_context);
        t_context.anim.update(t_context.frame_time_sec);
    }

    void PlayState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        StateBase::handleCloseEvents(t_context, t_event);

        if (const auto * keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            if (sf::Keyboard::Scancode::Space == keyPtr->scancode)
            {
                t_context.audio.play("pause");
                t_context.states.setChangePending(State::Pause);
                return;
            }
        }

        t_context.starship.handleEvent(t_context, t_event);
    }

    void PauseState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        StateBase::handleCloseEvents(t_context, t_event);

        if (const auto * keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            if (sf::Keyboard::Scancode::Space == keyPtr->scancode)
            {
                t_context.audio.play("pause");
                t_context.states.setChangePending(State::Play);
                return;
            }
        }
    }

    void PauseState::draw(Context & t_context)
    {
        StateBase::draw(t_context);
        t_context.window.draw(m_text);
    }

    StateMachine::StateMachine()
        : m_stateUPtr{ factory(State::Setup) }
    {}

    void StateMachine::changeIfPending(Context & t_context)
    {
        if (!m_statePending.has_value())
        {
            return;
        }

        m_stateUPtr->OnExit(t_context);
        m_stateUPtr = factory(m_statePending.value());
        m_statePending.reset();
        m_stateUPtr->OnEnter(t_context);
    }

    std::unique_ptr<IState> StateMachine::factory(const State t_state)
    {
        // clang-format off
        switch (t_state)
        {
            case State::Setup:   { return std::make_unique<SetupState>(); }
            case State::Start:   { return std::make_unique<StartState>(); }
            case State::Play:    { return std::make_unique<PlayState>();  }
            case State::Pause:   { return std::make_unique<PauseState>(); }
            case State::End:     { return std::make_unique<EndState>();   }
            case State::Teardown:
            default:             { return std::make_unique<TeardownState>(); }
        }
        // clang-format on
    }

} // namespace blast4
