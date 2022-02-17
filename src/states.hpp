#ifndef BLAST4_STATES_HPP
#define BLAST4_STATES_HPP

#include "context.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Window/Event.hpp>

namespace blast4
{
    enum class State
    {
        Setup,
        StartGame,
        Play,
        EndGame,
        Teardown
    };

    struct IState
    {
        virtual ~IState() {}

        virtual State which() const = 0;
        virtual void OnEnter(Context & context) = 0;
        virtual void OnExit(Context & context) = 0;
        virtual void update(Context & context) = 0;
        virtual void draw(Context & context) = 0;
        virtual void handleEvent(Context & context, const sf::Event & event) = 0;
    };

    class StateBase : public IState
    {
      public:
        StateBase() {}
        virtual ~StateBase() {}

        // prevent all copy and assignment
        StateBase(const StateBase &) = delete;
        StateBase(StateBase &&) = delete;
        StateBase operator=(const StateBase &) = delete;
        StateBase operator=(StateBase &&) = delete;

        void OnEnter(Context &) override {}
        void OnExit(Context &) override {}
        void update(Context &) override {}
        void draw(Context &) override {}
        void handleEvent(Context &, const sf::Event &) override {}
    };

    // do-nothing placeholder state for when the app is initializing
    class SetupState : public StateBase
    {
      public:
        virtual ~SetupState() {}
        State which() const override { return State::Setup; }
    };

    class StartGameState : public StateBase
    {
      public:
        virtual ~StartGameState() {}
        State which() const override { return State::StartGame; }
    };

    class PlayState : public StateBase
    {
      public:
        virtual ~PlayState() {}
        State which() const override { return State::Setup; }
        void draw(Context & context) override;
        void update(Context & context) override;
        void handleEvent(Context & context, const sf::Event & event) override;
    };

    class EndGameState : public StateBase
    {
      public:
        virtual ~EndGameState() {}
        State which() const override { return State::EndGame; }
    };

    // do-nothing placeholder state for when the app is exiting
    class TeardownState : public StateBase
    {
      public:
        virtual ~TeardownState() {}
        State which() const override { return State::Teardown; }
    };

    class StateMachine
    {
      public:
        StateMachine();

      public:
        State which() const { return m_stateUPtr->which(); }
        IState & state() { return *m_stateUPtr; }
        void setChangePending(const State nextState) { m_statePending = nextState; }
        void changeIfPending(Context & context);
        static std::unique_ptr<IState> factory(const State state);

      private:
        // must always contain a valid state
        std::unique_ptr<IState> m_stateUPtr;

        std::optional<State> m_statePending;
    };

} // namespace blast4

#endif // BLAST4_STATES_HPP
