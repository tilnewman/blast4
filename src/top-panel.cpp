#include "top-panel.hpp"

#include "board.hpp"
#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    TopPanel::TopPanel()
        : m_windowSize()
        , m_boardRect()
        , m_titleRect()
        , m_scoreRect()
        , m_ammoRect()
        , m_titleFont()
        , m_titleText()
        , m_generalFont()
        , m_ammoText()
        , m_scoreText()
    {}

    void TopPanel::setup(Context & context)
    {
        m_titleFont.loadFromFile("media/font/game-over/game-over.ttf");
        m_generalFont.loadFromFile("media/font/d-din/d-din.ttf");

        m_windowSize = sf::Vector2f{ context.window.getSize() };
        m_boardRect = context.board.rect();

        const float boardBorderPad = (m_windowSize.x * context.settings.border_pad_ratio);
        const float pad = std::floor(m_windowSize.y * 0.015f);

        m_titleRect.top = pad;
        m_titleRect.height = std::floor(m_boardRect.top - (pad * 2.0f));
        m_titleRect.height -= boardBorderPad;
        m_titleRect.width = std::floor(m_windowSize.x * 0.5f);
        m_titleRect.left = ((m_windowSize.x * 0.5f) - (m_titleRect.width * 0.5f));

        m_titleText.setFont(m_titleFont);
        m_titleText.setCharacterSize(200);
        m_titleText.setFillColor(context.settings.title_color);
        m_titleText.setString("BLAST4");
        m_titleText.setStyle(sf::Text::Bold);
        util::setOriginToPosition(m_titleText);
        util::fitAndCenterInside(m_titleText, m_titleRect);
        m_titleText.setPosition(m_titleText.getPosition().x, m_titleRect.top);

        m_ammoRect.left = (m_boardRect.left - boardBorderPad);
        m_ammoRect.width = (m_windowSize.x * 0.5f);
        m_ammoRect.height = (m_windowSize.y * context.settings.text_size_ratio);
        m_ammoRect.top = (m_boardRect.top - (boardBorderPad + m_ammoRect.height));

        m_ammoText.setFont(m_generalFont);
        m_ammoText.setCharacterSize(99);
        m_ammoText.setFillColor(context.settings.text_color);
        m_ammoText.setString("10");
        util::setOriginToPosition(m_ammoText);
        util::fit(m_ammoText, m_ammoRect);
        m_ammoText.setPosition(util::position(m_ammoRect));

        m_scoreRect = m_ammoRect;

        m_scoreText = m_ammoText;
    }

    void TopPanel::draw(Context & context) const
    {
        context.window.draw(m_titleText);
        context.window.draw(m_ammoText);
        context.window.draw(m_scoreText);
    }

} // namespace blast4
