// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "aliens.hpp"
#include "check-macros.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "starship.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Board::Board()
        : m_windowSize()
        , m_shipLength(0.0f)
        , m_boardRect()
        , m_blockRects()
        , m_blockRectangles()
        , m_borderReectangle()
        , m_backgroundRectangle()
        , m_horizLanes()
        , m_vertLanes()
        , m_horizLaneLines()
        , m_vertLaneLines()
    {}

    void Board::setup(Context & t_context)
    {
        m_windowSize = sf::Vector2f{ t_context.window.getSize() };

        // the width of halls between blocks is also the size of the player's spaceship
        m_shipLength = (m_windowSize.x * t_context.settings.ship_size_ratio);
        m_shipLength = std::floor(m_shipLength);

        // full rect of the board
        {
            const float pad = std::floor(m_windowSize.y * t_context.settings.edge_pad_ratio);
            m_boardRect.size.x = std::floor(m_windowSize.x - (pad * 4.0f));
            m_boardRect.position.x = ((m_windowSize.x * 0.5f) - (m_boardRect.size.x * 0.5f));
            m_boardRect.size.y = std::floor(m_windowSize.y - (pad * 4.0f));
            m_boardRect.position.y = (m_windowSize.y - (pad + m_boardRect.size.y));
        }

        // blocks that make up the halls, that the ships moves between
        const sf::Vector2f blockCount{ t_context.settings.block_count };

        const sf::Vector2f shipHallSizeTotals{ ((blockCount.x + 1.0f) * m_shipLength),
                                               (blockCount.y + 1.0f) * m_shipLength };

        const sf::Vector2f blockSize{ (m_boardRect.size - shipHallSizeTotals) / blockCount };

        std::vector<sf::Vector2f> blockPositions;

        blockPositions.reserve(
            static_cast<std::size_t>(t_context.settings.block_count.x) *
            static_cast<std::size_t>(t_context.settings.block_count.y));

        for (int y = 0; y < t_context.settings.block_count.y; ++y)
        {
            for (int x = 0; x < t_context.settings.block_count.x; ++x)
            {
                sf::Vector2f pos{ m_boardRect.position + shipSize() };
                pos.x += (static_cast<float>(x) * (blockSize.x + m_shipLength));
                pos.y += (static_cast<float>(y) * (blockSize.y + m_shipLength));

                blockPositions.push_back(pos);
            }
        }

        // verts that draw the blocks
        m_blockRects.reserve(blockPositions.size());
        m_blockRectangles.reserve(blockPositions.size());
        for (const sf::Vector2f & blockPosition : blockPositions)
        {
            sf::RectangleShape rs;
            rs.setFillColor(t_context.settings.block_color);
            rs.setPosition(blockPosition);
            rs.setSize(blockSize);

            m_blockRectangles.push_back(rs);
            m_blockRects.push_back({ blockPosition, blockSize });
        }

        // verts that draw the background
        m_backgroundRectangle.setFillColor(t_context.settings.board_color);
        m_backgroundRectangle.setPosition(m_boardRect.position);
        m_backgroundRectangle.setSize(m_boardRect.size);

        // verts that draw the walls
        const float pad = (m_windowSize.x * t_context.settings.border_pad_ratio);

        sf::FloatRect wallRect;
        wallRect.position.x = (m_boardRect.position.x - pad);
        wallRect.position.y = (m_boardRect.position.y - pad);
        wallRect.size.x = (m_boardRect.size.x + pad + pad);
        wallRect.size.y = (m_boardRect.size.y + pad + pad);

        m_borderReectangle.setFillColor(t_context.settings.block_color);
        m_borderReectangle.setPosition(wallRect.position);
        m_borderReectangle.setSize(wallRect.size);

        // possible lanes lines of movement
        float lane = m_boardRect.position.x;
        while (lane < m_boardRect.size.x)
        {
            lane += (m_shipLength * 0.5f);
            m_horizLaneLines.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.x;
        }

        lane = m_boardRect.position.y;
        while (lane < m_boardRect.size.y)
        {
            lane += (m_shipLength * 0.5f);
            m_vertLaneLines.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.y;
        }

        for (int y = 0; y < (t_context.settings.block_count.y + 1); ++y)
        {
            sf::FloatRect rect = m_boardRect;

            rect.position.y =
                m_boardRect.position.y + (static_cast<float>(y) * (m_shipLength + blockSize.y));

            rect.size.y = m_shipLength;
            m_horizLanes.push_back(rect);
        }

        for (int x = 0; x < (t_context.settings.block_count.x + 1); ++x)
        {
            sf::FloatRect rect = m_boardRect;

            rect.position.x =
                m_boardRect.position.x + (static_cast<float>(x) * (m_shipLength + blockSize.x));

            rect.size.x = m_shipLength;
            m_vertLanes.push_back(rect);
        }
    }

    void Board::draw(Context & t_context) const
    {
        t_context.window.draw(m_borderReectangle);
        t_context.window.draw(m_backgroundRectangle);

        for (const sf::RectangleShape & rectangle : m_blockRectangles)
        {
            t_context.window.draw(rectangle);
        }
    }

    bool Board::isCollisionWithBlock(const sf::FloatRect & rect) const
    {
        for (const sf::FloatRect blockRect : m_blockRects)
        {
            if (blockRect.findIntersection(rect))
            {
                return true;
            }
        }

        return false;
    }

    bool Board::isCollisionWithBoardEdge(const sf::FloatRect & rect) const
    {
        if (rect.position.y < m_boardRect.position.y)
        {
            return true;
        }

        if (util::bottom(rect) > util::bottom(m_boardRect))
        {
            return true;
        }

        if (util::right(rect) > util::right(m_boardRect))
        {
            return true;
        }

        if (rect.position.x < m_boardRect.position.x)
        {
            return true;
        }

        return false;
    }

    float Board::findLaneLine(const std::vector<float> & lines, const float position) const
    {
        for (const float line : lines)
        {
            const float diff{ util::abs(position - line) };

            if (diff < 2.0f)
            {
                return line;
            }
        }

        return -1.0f;
    }

    sf::FloatRect
        Board::findLane(const std::vector<sf::FloatRect> & lanes, const sf::FloatRect & rect) const
    {
        for (const sf::FloatRect & laneRect : lanes)
        {
            if (laneRect.findIntersection(rect))
            {
                return laneRect;
            }
        }

        return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
    }

    sf::Vector2f Board::randomFreePosition(const Context & t_context) const
    {
        std::vector<sf::Vector2f> positions;

        for (const float vertLane : m_vertLaneLines)
        {
            for (const float horizLane : m_horizLaneLines)
            {
                const sf::Vector2f position{ horizLane, vertLane };
                const sf::FloatRect rect{ position - (shipSize() * 0.5f), shipSize() };

                if (t_context.aliens.isCollision(rect))
                {
                    continue;
                }

                if (t_context.starship.intersects(rect))
                {
                    continue;
                }

                positions.push_back(position);
            }
        }

        M_CHECK(!positions.empty(), "Error:  No free places to spawn on the board!");

        return t_context.random.from(positions);
    }

    sf::Vector2f Board::randomFreeFarPosition(const Context & t_context) const
    {
        std::vector<sf::Vector2f> positions;

        for (const float vertLane : m_vertLaneLines)
        {
            for (const float horizLane : m_horizLaneLines)
            {
                const sf::Vector2f position{ horizLane, vertLane };
                const sf::FloatRect rect{ position - (shipSize() * 0.5f), shipSize() };

                if (t_context.aliens.isCollision(rect))
                {
                    continue;
                }

                if (t_context.starship.intersects(rect))
                {
                    continue;
                }

                positions.push_back(position);
            }
        }

        M_CHECK(!positions.empty(), "Error:  No free places to spawn on the board!");

        const sf::Vector2f playerPos{ util::center(t_context.starship.globalBounds()) };

        std::sort(std::begin(positions), std::end(positions), [&](const auto A, const auto B) {
            const float distA{ std::abs(playerPos.x - A.x) + std::abs(playerPos.y - A.y) };
            const float distB{ std::abs(playerPos.x - B.x) + std::abs(playerPos.y - B.y) };
            return (distA > distB);
        });

        return *positions.begin();
    }

    sf::Vector2s Board::laneIndexes(const sf::Vector2f & position) const
    {
        sf::Vector2s indexes{ std::numeric_limits<std::size_t>::max(),
                              std::numeric_limits<std::size_t>::max() };

        for (std::size_t i = 0; i < m_horizLanes.size(); ++i)
        {
            if (m_horizLanes[i].contains(position))
            {
                indexes.x = i;
                break;
            }
        }

        for (std::size_t i = 0; i < m_vertLanes.size(); ++i)
        {
            if (m_vertLanes[i].contains(position))
            {
                indexes.y = i;
                break;
            }
        }

        return indexes;
    }

    std::vector<float> Board::findLaneLinesOtherThanHoriz(const std::size_t indexToAvoid) const
    {
        std::vector<float> laneLines;
        for (std::size_t i = 0; i < m_horizLaneLines.size(); ++i)
        {
            if (indexToAvoid != i)
            {
                laneLines.push_back(m_horizLaneLines[i]);
            }
        }

        return laneLines;
    }

    std::vector<float> Board::findLaneLinesOtherThanVert(const std::size_t indexToAvoid) const
    {
        std::vector<float> laneLines;
        for (std::size_t i = 0; i < m_vertLaneLines.size(); ++i)
        {
            if (indexToAvoid != i)
            {
                laneLines.push_back(m_vertLaneLines[i]);
            }
        }

        return laneLines;
    }

} // namespace blast4
