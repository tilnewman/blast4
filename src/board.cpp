#include "board.hpp"

#include "aliens.hpp"
#include "check-macros.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "starship.hpp"
#include "util.hpp"

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
            m_boardRect.width = std::floor(m_windowSize.x - (pad * 4.0f));
            m_boardRect.left = ((m_windowSize.x * 0.5f) - (m_boardRect.width * 0.5f));
            m_boardRect.height = std::floor(m_windowSize.y - (pad * 4.0f));
            m_boardRect.top = (m_windowSize.y - (pad + m_boardRect.height));
        }

        // blocks that make up the halls, that the ships moves between
        const sf::Vector2f blockCount{ context.settings.block_count };

        const sf::Vector2f shipHallSizeTotals{ ((blockCount.x + 1.0f) * m_shipLength),
                                               (blockCount.y + 1.0f) * m_shipLength };

        const sf::Vector2f blockSize{ (util::size(m_boardRect) - shipHallSizeTotals) / blockCount };

        std::vector<sf::Vector2f> blockPositions;
        for (int y = 0; y < context.settings.block_count.y; ++y)
        {
            for (int x = 0; x < context.settings.block_count.x; ++x)
            {
                sf::Vector2f pos{ util::position(m_boardRect) + shipSize() };

                pos.x += (static_cast<float>(x) * (blockSize.x + m_shipLength));
                pos.y += (static_cast<float>(y) * (blockSize.y + m_shipLength));

                blockPositions.push_back(pos);
            }
        }

        // verts that draw the board
        for (const sf::Vector2f & blockPosition : blockPositions)
        {
            const sf::FloatRect blockRect{ blockPosition, blockSize };
            util::appendQuadVerts(blockRect, m_blockVerts, context.settings.block_color);
            m_blockRects.push_back(blockRect);
        }

        m_backgroundVerts.resize(
            4, sf::Vertex(util::position(m_boardRect), context.settings.board_color));

        m_backgroundVerts[1].position += { m_boardRect.width, 0.0f };
        m_backgroundVerts[2].position += { m_boardRect.width, m_boardRect.height };
        m_backgroundVerts[3].position += { 0.0f, m_boardRect.height };

        {
            const sf::Vector2f pos = util::position(m_boardRect);
            const float pad = (m_windowSize.x * context.settings.border_pad_ratio);

            m_borderVerts.push_back({ { pos.x - pad, pos.y - pad } });
            m_borderVerts.push_back({ { (pos.x + m_boardRect.width + pad), pos.y - pad } });
            m_borderVerts.push_back({ { (pos.x + m_boardRect.width + pad), pos.y } });
            m_borderVerts.push_back({ { pos.x - pad, pos.y } });
            //
            m_borderVerts.push_back({ { pos.x + m_boardRect.width, pos.y - pad } });
            m_borderVerts.push_back({ { (pos.x + m_boardRect.width + pad), pos.y - pad } });

            m_borderVerts.push_back(
                { { (pos.x + m_boardRect.width + pad), (pos.y + m_boardRect.height + pad) } });

            m_borderVerts.push_back(
                { { (pos.x + m_boardRect.width), (pos.y + m_boardRect.height + pad) } });
            //
            m_borderVerts.push_back({ { pos.x - pad, (pos.y + m_boardRect.height) } });

            m_borderVerts.push_back(
                { { (pos.x + m_boardRect.width + pad), (pos.y + m_boardRect.height) } });

            m_borderVerts.push_back(
                { { (pos.x + m_boardRect.width + pad), (pos.y + m_boardRect.height + pad) } });
            m_borderVerts.push_back({ { pos.x - pad, (pos.y + m_boardRect.height + pad) } });
            //
            m_borderVerts.push_back({ { pos.x - pad, pos.y - pad } });
            m_borderVerts.push_back({ { pos.x, pos.y - pad } });
            m_borderVerts.push_back({ { pos.x, (pos.y + m_boardRect.height + pad) } });
            m_borderVerts.push_back({ { (pos.x - pad), (pos.y + m_boardRect.height + pad) } });

            for (sf::Vertex & vert : m_borderVerts)
            {
                vert.color = context.settings.block_color;
            }
        }

        // possible lanes lines of movement
        float lane = m_boardRect.left;
        while (lane < m_boardRect.width)
        {
            lane += (m_shipLength * 0.5f);
            m_horizLaneLines.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.x;
        }

        lane = m_boardRect.top;
        while (lane < m_boardRect.height)
        {
            lane += (m_shipLength * 0.5f);
            m_vertLaneLines.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.y;
        }

        // possible lanes of movement
        for (int y = 0; y < (context.settings.block_count.y + 1); ++y)
        {
            sf::FloatRect rect = m_boardRect;
            rect.top = m_boardRect.top + (static_cast<float>(y) * (m_shipLength + blockSize.y));
            rect.height = m_shipLength;
            m_horizLanes.push_back(rect);
        }

        for (int x = 0; x < (context.settings.block_count.x + 1); ++x)
        {
            sf::FloatRect rect = m_boardRect;
            rect.left = m_boardRect.left + (static_cast<float>(x) * (m_shipLength + blockSize.x));
            rect.width = m_shipLength;
            m_vertLanes.push_back(rect);
        }
    }

    void Board::draw(Context & context) const
    {
        context.window.draw(&m_backgroundVerts[0], m_backgroundVerts.size(), sf::Quads);
        context.window.draw(&m_borderVerts[0], m_borderVerts.size(), sf::Quads);
        context.window.draw(&m_blockVerts[0], m_blockVerts.size(), sf::Quads);
    }

    bool Board::isCollisionWithBlock(const sf::FloatRect & rect) const
    {
        for (const sf::FloatRect blockRect : m_blockRects)
        {
            if (blockRect.intersects(rect))
            {
                return true;
            }
        }

        return false;
    }

    bool Board::isCollisionWithBoardEdge(const sf::FloatRect & rect) const
    {
        if (rect.top < m_boardRect.top)
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

        if (rect.left < m_boardRect.left)
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
            if (laneRect.intersects(rect))
            {
                return laneRect;
            }
        }

        return { 0.0f, 0.0f, 0.0f, 0.0f };
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

} // namespace blast4
