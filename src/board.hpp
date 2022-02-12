#ifndef BLAST4_BOARD_HPP
#define BLAST4_BOARD_HPP

#include "context.hpp"

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace blast4
{

    class Board
    {
      public:
        Board();

        void setup(Context & context);
        void draw(Context & context);

      private:
        sf::Color m_boardColor;
        sf::Color m_gridColor;

        sf::Vector2f m_windowSize;
        float m_unitSize = 0.0f;
        sf::FloatRect m_boardRect;

        std::vector<sf::Vertex> m_blockVerts;
        std::vector<sf::Vertex> m_borderVerts;
        std::vector<sf::Vertex> m_backgroundVerts;
    };

} // namespace blast4

#endif // BLAST4_BOARD_HPP
