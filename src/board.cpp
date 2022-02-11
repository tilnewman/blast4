#include "board.hpp"

#include "settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Board::Board()
        : m_boardColor(sf::Color::Black)
        , m_gridColor(sf::Color::White)
        , m_windowSize()
        , m_gridSize(0.0f)
    {}

    void Board::setup(Context & context)
    {
        m_windowSize = sf::Vector2f{ context.window.getSize() };
        m_gridSize = (m_windowSize.x * context.settings.grid_size_ratio);
    }

} // namespace blast4
