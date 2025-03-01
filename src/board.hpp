#ifndef BLAST4_BOARD_HPP
#define BLAST4_BOARD_HPP

#include "context.hpp"
#include "sfml-util.hpp"

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

        [[nodiscard]] inline sf::FloatRect rect() const noexcept { return m_boardRect; }

        [[nodiscard]] inline sf::Vector2f shipSize() const noexcept
        {
            return { m_shipLength, m_shipLength };
        }

        [[nodiscard]] bool isCollisionWithBlock(const sf::FloatRect & t_rect) const;
        [[nodiscard]] bool isCollisionWithBoardEdge(const sf::FloatRect & t_rect) const;

        [[nodiscard]] inline bool isCollision(const sf::FloatRect & t_rect) const
        {
            return (isCollisionWithBlock(t_rect) || isCollisionWithBoardEdge(t_rect));
        }

        [[nodiscard]] inline float findLaneLineHoriz(const float t_position) const
        {
            return findLaneLine(m_horizLaneLines, t_position);
        }

        [[nodiscard]] inline float findLaneLineVert(const float t_position) const
        {
            return findLaneLine(m_vertLaneLines, t_position);
        }

        [[nodiscard]] inline sf::FloatRect findLaneHoriz(const sf::FloatRect & t_rect) const
        {
            return findLane(m_horizLanes, t_rect);
        }

        [[nodiscard]] inline sf::FloatRect findLaneVert(const sf::FloatRect & t_rect) const
        {
            return findLane(m_vertLanes, t_rect);
        }

        [[nodiscard]] sf::Vector2f randomFreePosition(const Context & t_context) const;
        [[nodiscard]] sf::Vector2f randomFreeFarPosition(const Context & t_context) const;
        [[nodiscard]] sf::Vector2s laneIndexes(const sf::Vector2f & t_position) const;

        [[nodiscard]] std::vector<float>
            findLaneLinesOtherThanHoriz(const std::size_t t_indexToAvoid) const;

        [[nodiscard]] std::vector<float>
            findLaneLinesOtherThanVert(const std::size_t t_indexToAvoid) const;

      private:
        [[nodiscard]] float
            findLaneLine(const std::vector<float> & t_lines, const float t_position) const;

        [[nodiscard]] sf::FloatRect
            findLane(const std::vector<sf::FloatRect> & t_lanes, const sf::FloatRect & t_rect) const;

      private:
        sf::Vector2f m_windowSize;
        float m_shipLength;
        sf::FloatRect m_boardRect;
        std::vector<sf::FloatRect> m_blockRects;

        std::vector<sf::RectangleShape> m_blockRectangles;
        sf::RectangleShape m_borderReectangle;
        sf::RectangleShape m_backgroundRectangle;

        std::vector<sf::FloatRect> m_horizLanes;
        std::vector<sf::FloatRect> m_vertLanes;

        std::vector<float> m_horizLaneLines;
        std::vector<float> m_vertLaneLines;
    };

} // namespace blast4

#endif // BLAST4_BOARD_HPP
