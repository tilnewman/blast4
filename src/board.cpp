#include "board.hpp"

#include "settings.hpp"
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
    {}

    void Board::setup(Context & context)
    {
        m_windowSize = sf::Vector2f{ context.window.getSize() };

        // the width of halls between blocks is also the size of the player's spaceship
        m_shipLength = (m_windowSize.x * context.settings.ship_size_ratio);
        m_shipLength = std::floor(m_shipLength);

        // full rect of the board
        const float pad = std::floor(m_windowSize.y * context.settings.edge_pad_ratio);
        m_boardRect.width = std::floor(m_windowSize.x - (pad * 4.0f));
        m_boardRect.left = ((m_windowSize.x * 0.5f) - (m_boardRect.width * 0.5f));
        m_boardRect.height = std::floor(m_windowSize.y - (pad * 4.0f));
        m_boardRect.top = (m_windowSize.y - (pad + m_boardRect.height));

        // blocks that make up the halls, or blocks the ship moves between
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

        m_borderVerts.resize(
            5, sf::Vertex(util::position(m_boardRect), context.settings.block_color));

        m_borderVerts[1].position += { m_boardRect.width, 0.0f };
        m_borderVerts[2].position += { m_boardRect.width, m_boardRect.height };
        m_borderVerts[3].position += { 0.0f, m_boardRect.height };

        m_backgroundVerts = m_borderVerts;
        m_backgroundVerts[0].color = context.settings.board_color;
        m_backgroundVerts[1].color = context.settings.board_color;
        m_backgroundVerts[2].color = context.settings.board_color;
        m_backgroundVerts[3].color = context.settings.board_color;
        m_backgroundVerts.pop_back();

        // possible lanes of movement
        float lane = m_boardRect.left;
        while (lane < m_boardRect.width)
        {
            lane += (m_shipLength * 0.5f);
            m_horizLanes.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.x;
        }

        lane = m_boardRect.top;
        while (lane < m_boardRect.height)
        {
            lane += (m_shipLength * 0.5f);
            m_vertLanes.push_back(lane);
            lane += (m_shipLength * 0.5f);
            lane += blockSize.y;
        }
    }

    void Board::draw(Context & context) const
    {
        context.window.draw(&m_backgroundVerts[0], m_backgroundVerts.size(), sf::Quads);
        context.window.draw(&m_borderVerts[0], m_borderVerts.size(), sf::LinesStrip);
        context.window.draw(&m_blockVerts[0], m_blockVerts.size(), sf::Quads);
    }

    float Board::findFirstWithinRange(const std::vector<float> & lanes, const float position) const
    {
        for (const float lane : lanes)
        {
            const float diff{ util::abs(position - lane) };

            if (diff < 2.0f)
            {
                return lane;
            }
        }

        return -1.0f;
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

} // namespace blast4
