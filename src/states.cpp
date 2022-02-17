// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "states.hpp"

#include "aliens.hpp"
#include "ammo.hpp"
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

    void PlayState::draw(Context & context)
    {
        context.board.draw(context);
        context.panel.draw(context);
        context.ammo.draw(context);
        context.aliens.draw(context);
        context.starship.draw(context);
        context.bullets.draw(context);
    }

    void PlayState::update(Context & context)
    {
        context.starship.update(context);
        context.aliens.update(context);
        context.bullets.update(context);
        context.panel.update(context);
    }

    void PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            context.window.close();
            return;
        }

        context.starship.handleEvent(context, event);

        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (sf::Keyboard::Escape == event.key.code)
        {
            context.window.close();
            return;
        }
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
        m_stateUPtr.release();
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
            default:
            case State::Teardown: { return std::make_unique<TeardownState>(); }
        }
        // clang-format on
    }

} // namespace blast4
