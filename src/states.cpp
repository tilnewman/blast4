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
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    void StateBase::update(Context & context) { context.panel.update(context); }

    void StateBase::draw(Context & context)
    {
        context.board.draw(context);
        context.panel.draw(context);
        context.ammo.draw(context);
        context.aliens.draw(context);
        context.starship.draw(context);
        context.bullets.draw(context);
        context.anim.draw(context.window, {});
    }

    void StateBase::handleCloseEvents(Context & context, const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            context.window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::Escape == event.key.code)
            {
                context.window.close();
            }
        }
    }

    void
        StateBase::setupTextMessage(Context & context, const std::string & message, sf::Text & text)
    {
        text.setFont(context.panel.titleFont());
        text.setFillColor(context.settings.title_color);
        text.setCharacterSize(200);
        text.setString(message);
        util::setOriginToPosition(text);

        const sf::FloatRect boardRect = context.board.rect();
        const float height{ boardRect.height * 0.2f };
        const sf::FloatRect fitRect{ 0.0f, 0.0f, boardRect.width, height };
        util::fit(text, fitRect);

        text.setPosition(util::center(boardRect) - (util::size(text) * 0.5f));
    }

    void StartState::OnEnter(Context & context)
    {
        m_timerSec = 5.0f;
        context.audio.play("game-start");
    }

    void StartState::update(Context & context)
    {
        m_timerSec -= context.frame_time_sec;
        if (m_timerSec < 0.0f)
        {
            context.states.setChangePending(State::Play);
        }
    }

    void EndState::OnEnter(Context & context)
    {
        m_timerSec = 5.0f;
        context.audio.play("game-end");
    }

    void EndState::update(Context & context)
    {
        context.anim.update(context.frame_time_sec);

        m_timerSec -= context.frame_time_sec;
        if (m_timerSec < 0.0f)
        {
            context.states.setChangePending(State::Teardown);
        }
    }

    void EndState::draw(Context & context)
    {
        context.board.draw(context);
        context.panel.draw(context);
        context.ammo.draw(context);
        context.aliens.draw(context);
        context.anim.draw(context.window, {});
    }

    void PlayState::update(Context & context)
    {
        StateBase::update(context);
        context.starship.update(context);
        context.aliens.update(context);
        context.bullets.update(context);
        context.anim.update(context.frame_time_sec);
    }

    void PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        StateBase::handleCloseEvents(context, event);

        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::Space == event.key.code)
            {
                context.audio.play("pause");
                context.states.setChangePending(State::Pause);
                return;
            }
        }

        context.starship.handleEvent(context, event);
    }

    void PauseState::handleEvent(Context & context, const sf::Event & event)
    {
        StateBase::handleCloseEvents(context, event);

        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::Space == event.key.code)
            {
                context.audio.play("pause");
                context.states.setChangePending(State::Play);
                return;
            }
        }
    }

    void PauseState::draw(Context & context)
    {
        StateBase::draw(context);
        context.window.draw(m_text);
    }

    StateMachine::StateMachine()
        : m_stateUPtr(factory(State::Setup))
    {}

    void StateMachine::changeIfPending(Context & context)
    {
        if (!m_statePending.has_value())
        {
            return;
        }

        m_stateUPtr->OnExit(context);
        m_stateUPtr.reset();
        m_stateUPtr = factory(m_statePending.value());
        m_statePending.reset();
        m_stateUPtr->OnEnter(context);
    }

    std::unique_ptr<IState> StateMachine::factory(const State state)
    {
        // clang-format off
        switch (state)
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
