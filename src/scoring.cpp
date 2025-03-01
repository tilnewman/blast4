// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "scoring.hpp"

#include "aliens.hpp"
#include "ammo.hpp"
#include "context.hpp"
#include "game.hpp"
#include "settings.hpp"

namespace blast4
{

    Scoring::Scoring()
        : m_scoreBefore(0)
        , m_scoreAfter(0)
    {}

    void Scoring::preTurn(const Context & context) { m_scoreBefore = context.game.score; }

    void Scoring::postTurn(Context & context)
    {
        m_scoreAfter = context.game.score;

        if (hasScoreIncreasedBy(context.settings.score_per_difficulty))
        {
            context.aliens.placeRandom(context);
            context.settings.ship_speed += 15.0f;
            context.settings.bullet_speed += 15.0f;
        }

        if (hasScoreIncreasedBy(context.settings.score_per_difficulty * 2))
        {
            context.ammo.placeRandom(context);
        }
    }

} // namespace blast4
