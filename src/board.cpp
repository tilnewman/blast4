#include "board.hpp"

#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Board::Board()
        : m_windowSize()
        , m_unitSize(0.0f)
        , m_boardRect()
        , m_blockVerts()
        , m_borderVerts()
        , m_backgroundVerts()
        , m_horizLanes()
        , m_vertLanes()
    {}

    void Board::setup(Context & context)
    {
        m_windowSize = sf::Vector2f{ context.window.getSize() };
        m_unitSize = (m_windowSize.x * context.settings.unit_size_ratio);
        m_unitSize = std::floor(m_unitSize);

        // this initial board rect is only a temp estimate, see actual rect set below
        const float pad = std::floor(m_windowSize.y * context.settings.edge_pad_ratio);
        m_boardRect.top = pad;
        m_boardRect.width = m_windowSize.x - (pad * 5.0f);
        m_boardRect.height = m_windowSize.y - (pad * 2.0f);
        m_boardRect.left = ((m_windowSize.x * 0.5f) - (m_boardRect.width * 0.5f));

        // blocks that make up the halls, or blocks the ship moves between
        const sf::Vector2f blockSize{ std::floor(m_unitSize * 2.0f),
                                      std::floor(m_unitSize * 3.0f) };

        std::vector<sf::Vector2f> blockPositions;

        sf::Vector2f extents{ 0.0f, 0.0f };
        {
            sf::Vector2f pos{ 0.0f, 0.0f };
            while (pos.y < (m_boardRect.height - blockSize.y))
            {
                while (pos.x < (m_boardRect.width - blockSize.x))
                {
                    blockPositions.push_back(pos);
                    pos.x += blockSize.x;
                    pos.x += m_unitSize;
                    extents.x = pos.x;
                }

                pos.x = 0.0f;

                pos.y += blockSize.y;
                pos.y += m_unitSize;
                extents.y = pos.y;
            }
        }

        // set the actual board rect based on the number of blocks that fit the estimate above
        const sf::Vector2f boardSize{ (extents.x - m_unitSize), (extents.y - m_unitSize) };
        const sf::Vector2f boardPos = ((m_windowSize * 0.5f) - (boardSize * 0.5f));
        m_boardRect = { boardPos, boardSize };

        // verts
        for (const sf::Vector2f & blockPosition : blockPositions)
        {
            util::appendQuadVerts(
                { (boardPos + blockPosition), blockSize },
                m_blockVerts,
                context.settings.block_color);
        }

        m_borderVerts.resize(
            5, sf::Vertex(util::position(m_boardRect), context.settings.block_color));

        m_borderVerts[1].position += { m_boardRect.width, 0.0f };
        m_borderVerts[2].position += { m_boardRect.width, m_boardRect.height };
        m_borderVerts[3].position += { 0.0f, m_boardRect.height };

        m_backgroundVerts = m_borderVerts;
        m_backgroundVerts[0].color = context.settings.background_color;
        m_backgroundVerts[1].color = context.settings.background_color;
        m_backgroundVerts[2].color = context.settings.background_color;
        m_backgroundVerts[3].color = context.settings.background_color;
        m_backgroundVerts.pop_back();

        // possible lanes of movement
        float lane = m_boardRect.left;
        while (lane < m_boardRect.width)
        {
            lane += blockSize.x;
            lane += (m_unitSize * 0.5f);
            m_horizLanes.push_back(lane);
            lane += (m_unitSize * 0.5f);
        }

        lane = m_boardRect.top;
        while (lane < m_boardRect.height)
        {
            lane += blockSize.y;
            lane += (m_unitSize * 0.5f);
            m_vertLanes.push_back(lane);
            lane += (m_unitSize * 0.5f);
        }
    }

    void Board::draw(Context & context)
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

} // namespace blast4
