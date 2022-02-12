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

        const sf::Vector2f startPosition() const
        {
            return { m_horizLanes.at(0), m_vertLanes.at(0) };
        }

        const sf::Vector2f unitSize() const { return { m_unitSize, m_unitSize }; }

      private:
        sf::Vector2f m_windowSize;
        float m_unitSize = 0.0f;
        sf::FloatRect m_boardRect;

        std::vector<sf::Vertex> m_blockVerts;
        std::vector<sf::Vertex> m_borderVerts;
        std::vector<sf::Vertex> m_backgroundVerts;

        std::vector<float> m_horizLanes;
        std::vector<float> m_vertLanes;
    };

} // namespace blast4

#endif // BLAST4_BOARD_HPP
