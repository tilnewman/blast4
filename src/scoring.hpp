#ifndef BLAST4_SCORING_HPP
#define BLAST4_SCORING_HPP

namespace blast4
{
    struct Context;

    class Scoring
    {
      public:
        Scoring();

        void preTurn(const Context & t_context);
        void postTurn(Context & t_context);

      private:
        [[nodiscard]] inline bool hasScoreIncreasedBy(const int t_amount) const noexcept
        {
            return ((m_scoreAfter / t_amount) > (m_scoreBefore / t_amount));
        }

      private:
        int m_scoreBefore;
        int m_scoreAfter;
    };

} // namespace blast4

#endif // BLAST4_SCORING_HPP
