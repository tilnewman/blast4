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
        void draw(Context & context) const;

        const sf::FloatRect rect() const { return m_boardRect; }

        const sf::Vector2f shipSize() const { return { m_shipLength, m_shipLength }; }

        bool isCollisionWithBlock(const sf::FloatRect & rect) const;
        bool isCollisionWithBoardEdge(const sf::FloatRect & rect) const;

        bool isCollision(const sf::FloatRect & rect) const
        {
            return (isCollisionWithBlock(rect) || isCollisionWithBoardEdge(rect));
        }

        float findLaneLineHoriz(const float position) const
        {
            return findLaneLine(m_horizLaneLines, position);
        }

        float findLaneLineVert(const float position) const
        {
            return findLaneLine(m_vertLaneLines, position);
        }

        const sf::FloatRect findLaneHoriz(const sf::FloatRect & rect) const
        {
            return findLane(m_horizLanes, rect);
        }

        const sf::FloatRect findLaneVert(const sf::FloatRect & rect) const
        {
            return findLane(m_vertLanes, rect);
        }

        const sf::Vector2f randomPosition(const Context & context) const;

      private:
        float findLaneLine(const std::vector<float> & lines, const float position) const;

        const sf::FloatRect
            findLane(const std::vector<sf::FloatRect> & lanes, const sf::FloatRect & rect) const;

      private:
        sf::Vector2f m_windowSize;
        float m_shipLength = 0.0f;
        sf::FloatRect m_boardRect;
        std::vector<sf::FloatRect> m_blockRects;

        std::vector<sf::Vertex> m_blockVerts;
        std::vector<sf::Vertex> m_borderVerts;
        std::vector<sf::Vertex> m_backgroundVerts;

        std::vector<sf::FloatRect> m_horizLanes;
        std::vector<sf::FloatRect> m_vertLanes;

        std::vector<float> m_horizLaneLines;
        std::vector<float> m_vertLaneLines;
    };

} // namespace blast4

#endif // BLAST4_BOARD_HPP
