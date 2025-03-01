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
        , m_blockVerts()
        , m_borderVerts()
        , m_backgroundVerts()
        , m_horizLanes()
        , m_vertLanes()
        , m_horizLaneLines()
        , m_vertLaneLines()
    {}

    void Board::setup(Context & context)
    {
        m_windowSize = sf::Vector2f{ context.window.getSize() };

        // the width of halls between blocks is also the size of the player's spaceship
        m_shipLength = (m_windowSize.x * context.settings.ship_size_ratio);
        m_shipLength = std::floor(m_shipLength);

        // full rect of the board
        {
            const float pad = std::floor(m_windowSize.y * context.settings.edge_pad_ratio);
            m_boardRect.size.x = std::floor(m_windowSize.x - (pad * 4.0f));
            m_boardRect.position.x = ((m_windowSize.x * 0.5f) - (m_boardRect.size.x * 0.5f));
            m_boardRect.size.y = std::floor(m_windowSize.y - (pad * 4.0f));
            m_boardRect.position.y = (m_windowSize.y - (pad + m_boardRect.size.y));
        }

        // blocks that make up the halls, that the ships moves between
        const sf::Vector2f blockCount{ context.settings.block_count };

        const sf::Vector2f shipHallSizeTotals{ ((blockCount.x + 1.0f) * m_shipLength),
                                               (blockCount.y + 1.0f) * m_shipLength };

        const sf::Vector2f blockSize{ (m_boardRect.size - shipHallSizeTotals) / blockCount };

        std::vector<sf::Vector2f> blockPositions;
        for (int y = 0; y < context.settings.block_count.y; ++y)
        {
            for (int x = 0; x < context.settings.block_count.x; ++x)
            {
                sf::Vector2f pos{ m_boardRect.position + shipSize() };

                pos.x += (static_cast<float>(x) * (blockSize.x + m_shipLength));
                pos.y += (static_cast<float>(y) * (blockSize.y + m_shipLength));

                blockPositions.push_back(pos);
            }
        }

        // verts that draw the blocks
        for (const sf::Vector2f & blockPosition : blockPositions)
        {
            const sf::FloatRect blockRect{ blockPosition, blockSize };
            util::appendTriangleVerts(blockRect, m_blockVerts, context.settings.block_color);
            m_blockRects.push_back(blockRect);
        }

        // verts that draw the background
        util::appendTriangleVerts(m_boardRect, m_backgroundVerts, context.settings.board_color);

        // verts that draw the walls
        const sf::Vector2f pos = m_boardRect.position;
        const float pad = (m_windowSize.x * context.settings.border_pad_ratio);

        sf::FloatRect topWallRect;
        topWallRect.position.x = (pos.x - pad);
        topWallRect.position.y = (pos.y - pad);
        topWallRect.size.x = (m_boardRect.size.x + pad);
        topWallRect.size.y = pad;
        util::appendTriangleVerts(topWallRect, m_backgroundVerts, context.settings.block_color);

        sf::FloatRect rightWallRect;
        rightWallRect.position.x = (pos.x + m_boardRect.size.x);
        rightWallRect.position.y = (pos.y - pad);
        rightWallRect.size.x = pad;
        rightWallRect.size.y = (m_boardRect.size.y + pad);
        util::appendTriangleVerts(rightWallRect, m_backgroundVerts, context.settings.block_color);

        sf::FloatRect bottomWallRect;
        bottomWallRect.position.x = (pos.x - pad);
        bottomWallRect.position.y = (pos.y + m_boardRect.size.y);
        bottomWallRect.size.x = (m_boardRect.size.x + pad + pad);
        bottomWallRect.size.y = pad;
        util::appendTriangleVerts(bottomWallRect, m_backgroundVerts, context.settings.block_color);

        sf::FloatRect leftWallRect;
        leftWallRect.position.x = (pos.x - pad);
        leftWallRect.position.y = pos.y;
        leftWallRect.size.x = pad;
        leftWallRect.size.y = m_boardRect.size.y;
        util::appendTriangleVerts(leftWallRect, m_backgroundVerts, context.settings.block_color);

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

        for (int y = 0; y < (context.settings.block_count.y + 1); ++y)
        {
            sf::FloatRect rect = m_boardRect;

            rect.position.y =
                m_boardRect.position.y + (static_cast<float>(y) * (m_shipLength + blockSize.y));

            rect.size.y = m_shipLength;
            m_horizLanes.push_back(rect);
        }

        for (int x = 0; x < (context.settings.block_count.x + 1); ++x)
        {
            sf::FloatRect rect = m_boardRect;

            rect.position.x =
                m_boardRect.position.x + (static_cast<float>(x) * (m_shipLength + blockSize.x));

            rect.size.x = m_shipLength;
            m_vertLanes.push_back(rect);
        }
    }

    void Board::draw(Context & context) const
    {
        if (!m_backgroundVerts.empty())
        {
            context.window.draw(
                &m_backgroundVerts[0], m_backgroundVerts.size(), sf::PrimitiveType::Triangles);
        }

        if (!m_borderVerts.empty())
        {
            context.window.draw(
                &m_borderVerts[0], m_borderVerts.size(), sf::PrimitiveType::Triangles);
        }

        if (!m_blockVerts.empty())
        {
            context.window.draw(
                &m_blockVerts[0], m_blockVerts.size(), sf::PrimitiveType::Triangles);
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

    const sf::FloatRect
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

    const sf::Vector2f Board::randomFreePosition(const Context & context) const
    {
        std::vector<sf::Vector2f> positions;

        for (const float vertLane : m_vertLaneLines)
        {
            for (const float horizLane : m_horizLaneLines)
            {
                const sf::Vector2f position{ horizLane, vertLane };
                const sf::FloatRect rect{ position - (shipSize() * 0.5f), shipSize() };

                if (context.aliens.isCollision(rect))
                {
                    continue;
                }

                if (context.starship.intersects(rect))
                {
                    continue;
                }

                positions.push_back(position);
            }
        }

        M_CHECK(!positions.empty(), "Error:  No free places to spawn on the board!");

        return context.random.from(positions);
    }

    const sf::Vector2f Board::randomFreeFarPosition(const Context & context) const
    {
        std::vector<sf::Vector2f> positions;

        for (const float vertLane : m_vertLaneLines)
        {
            for (const float horizLane : m_horizLaneLines)
            {
                const sf::Vector2f position{ horizLane, vertLane };
                const sf::FloatRect rect{ position - (shipSize() * 0.5f), shipSize() };

                if (context.aliens.isCollision(rect))
                {
                    continue;
                }

                if (context.starship.intersects(rect))
                {
                    continue;
                }

                positions.push_back(position);
            }
        }

        M_CHECK(!positions.empty(), "Error:  No free places to spawn on the board!");

        const sf::Vector2f playerPos{ util::center(context.starship.globalBounds()) };

        std::sort(std::begin(positions), std::end(positions), [&](const auto A, const auto B) {
            const float distA{ std::abs(playerPos.x - A.x) + std::abs(playerPos.y - A.y) };
            const float distB{ std::abs(playerPos.x - B.x) + std::abs(playerPos.y - B.y) };
            return (distA > distB);
        });

        return *positions.begin();
    }

    const sf::Vector2s Board::laneIndexes(const sf::Vector2f & position) const
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

    const std::vector<float>
        Board::findLaneLinesOtherThanHoriz(const std::size_t indexToAvoid) const
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

    const std::vector<float> Board::findLaneLinesOtherThanVert(const std::size_t indexToAvoid) const
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
