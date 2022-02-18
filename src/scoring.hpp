#ifndef BLAST4_SCORING_HPP
#define BLAST4_SCORING_HPP

namespace blast4
{
    struct Context;

    class Scoring
    {
      public:
        Scoring();

        void preTurn(const Context & context);
        void postTurn(Context & context);

      private:
        bool hasScoreIncreasedBy(const int amount) const;

      private:
        int m_scoreBefore;
        int m_scoreAfter;
    };

} // namespace blast4

#endif // BLAST4_SCORING_HPP
