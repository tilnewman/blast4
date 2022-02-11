#ifndef BLAST4_BOARD_HPP
#define BLAST4_BOARD_HPP

#include "context.hpp"

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace blast4
{

    class Board
    {
      public:
        Board();

        void setup(Context & context);

      private:
        sf::Color m_boardColor;
        sf::Color m_gridColor;

        sf::Vector2f m_windowSize;
        float m_gridSize = 0.0f;
        sf::FloatRect m_gridRect;
    };

} // namespace blast4

#endif // BLAST4_BOARD_HPP
