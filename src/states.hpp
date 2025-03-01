#ifndef BLAST4_STATES_HPP
#define BLAST4_STATES_HPP

#include "sfml-defaults.hpp"
#include "context.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

namespace blast4
{
    enum class State
    {
        Setup,
        Start,
        Play,
        Pause,
        End,
        Teardown
    };

    struct IState
    {
        virtual ~IState() {}

        virtual State which() const = 0;
        virtual void OnEnter(Context & t_context) = 0;
        virtual void OnExit(Context & t_context) = 0;
        virtual void update(Context & t_context) = 0;
        virtual void draw(Context & t_context) = 0;
        virtual void handleEvent(Context & t_context, const sf::Event & t_event) = 0;
    };

    class StateBase : public IState
    {
      public:
        StateBase() {}
        virtual ~StateBase() override {}

        void OnEnter(Context &) override {}
        void OnExit(Context &) override {}
        void update(Context &) override;
        void draw(Context &) override;
        void handleEvent(Context &, const sf::Event &) override {}
        void handleCloseEvents(Context & t_context, const sf::Event & t_event);

      protected:
        static void
            setupTextMessage(Context & t_context, const std::string & t_message, sf::Text & t_text);
    };

    // do-nothing placeholder state for when the app is initializing
    class SetupState : public StateBase
    {
      public:
        virtual ~SetupState() override {}
        State which() const override { return State::Setup; }
    };

    class StartState : public StateBase
    {
      public:
        virtual ~StartState() override {}
        State which() const override { return State::Start; }
        void OnEnter(Context &) override;
        void update(Context &) override;

      private:
        float m_timerSec{ 0.0f };
    };

    class PlayState : public StateBase
    {
      public:
        virtual ~PlayState() override {}
        State which() const override { return State::Setup; }
        void update(Context & t_context) override;
        void handleEvent(Context & t_context, const sf::Event & t_event) override;
    };

    class PauseState : public StateBase
    {
      public:
        PauseState()
            : StateBase()
            , m_text(util::SfmlDefaults::instance().font())
        {}

        virtual ~PauseState() override {}
        State which() const override { return State::Pause; }
        void handleEvent(Context & t_context, const sf::Event & t_event) override;
        void draw(Context & t_context) override;
        void OnEnter(Context & t_context) override { setupTextMessage(t_context, "PAUSE", m_text); }

      private:
        sf::Text m_text;
    };

    class EndState : public StateBase
    {
      public:
        virtual ~EndState() override {}
        State which() const override { return State::End; }
        void OnEnter(Context &) override;
        void update(Context &) override;
        void draw(Context &) override;

      private:
        float m_timerSec{ 0.0f };
    };

    // do-nothing placeholder state for when the app is exiting
    class TeardownState : public StateBase
    {
      public:
        virtual ~TeardownState() override {}
        State which() const override { return State::Teardown; }
        void draw(Context &) override {}
    };

    class StateMachine
    {
      public:
        StateMachine();

      public:
        [[nodiscard]] inline State which() const { return m_stateUPtr->which(); }
        [[nodiscard]] inline IState & state() { return *m_stateUPtr; }
        inline void setChangePending(const State t_nextState) noexcept { m_statePending = t_nextState; }
        void changeIfPending(Context & t_context);
        static std::unique_ptr<IState> factory(const State t_state);

      private:
        // must always point to a valid state
        std::unique_ptr<IState> m_stateUPtr;

        //only has a value when a new state is pending
        std::optional<State> m_statePending;
    };

} // namespace blast4

#endif // BLAST4_STATES_HPP
