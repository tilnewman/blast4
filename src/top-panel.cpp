// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "top-panel.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "game.hpp"
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
        , m_titleText(m_titleFont)
        , m_generalFont()
        , m_ammoText(m_generalFont)
        , m_scoreText(m_generalFont)
    {}

    void TopPanel::setup(Context & context)
    {
        M_CHECK(
            m_titleFont.openFromFile(
                (context.settings.media_path / "font/game-over/game-over.ttf").string()),
            "Failed to load title font");

        M_CHECK(
            m_generalFont.openFromFile(
                (context.settings.media_path / "font/d-din/d-din.ttf").string()),
            "Failed to load general font");

        m_windowSize = sf::Vector2f{ context.window.getSize() };
        m_boardRect = context.board.rect();

        const float boardBorderPad = (m_windowSize.x * context.settings.border_pad_ratio);
        const float pad = std::floor(m_windowSize.y * 0.015f);

        m_titleRect.position.y = pad;
        m_titleRect.size.y = std::floor(m_boardRect.position.y - (pad * 2.0f));
        m_titleRect.size.y -= boardBorderPad;
        m_titleRect.size.x = std::floor(m_windowSize.x * 0.5f);
        m_titleRect.position.x = ((m_windowSize.x * 0.5f) - (m_titleRect.size.x * 0.5f));

        m_titleText.setFont(m_titleFont);
        m_titleText.setCharacterSize(200);
        m_titleText.setFillColor(context.settings.title_color);
        m_titleText.setString("BLAST4");
        m_titleText.setStyle(sf::Text::Bold);
        util::setOriginToPosition(m_titleText);
        util::fitAndCenterInside(m_titleText, m_titleRect);
        m_titleText.setPosition({ m_titleText.getPosition().x, m_titleRect.position.y });

        m_ammoRect.position.x = m_boardRect.position.x;
        m_ammoRect.size.x = (m_windowSize.x * 0.5f);
        m_ammoRect.size.y = (m_windowSize.y * context.settings.text_size_ratio);
        m_ammoRect.position.y = (m_boardRect.position.y - (boardBorderPad + m_ammoRect.size.y));

        m_ammoText.setFont(m_generalFont);
        m_ammoText.setCharacterSize(99);
        m_ammoText.setFillColor(context.settings.text_color);
        m_ammoText.setString("10");
        util::setOriginToPosition(m_ammoText);
        util::fit(m_ammoText, m_ammoRect);
        m_ammoText.setPosition(m_ammoRect.position);

        m_scoreRect = m_ammoRect;

        m_scoreText = m_ammoText;
    }

    void TopPanel::update(Context & context)
    {
        m_ammoText.setString(std::to_string(context.game.ammo));
        util::setOriginToPosition(m_ammoText);
        util::fit(m_ammoText, m_ammoRect);
        m_ammoText.setPosition(m_ammoRect.position);

        m_scoreText.setString(std::to_string(context.game.score));
        util::setOriginToPosition(m_scoreText);
        util::fit(m_scoreText, m_scoreRect);

        m_scoreText.setPosition({ (util::right(m_boardRect) - m_scoreText.getGlobalBounds().size.x),
                                  m_scoreRect.position.y });
    }

    void TopPanel::draw(Context & context) const
    {
        context.window.draw(m_titleText);
        context.window.draw(m_ammoText);
        context.window.draw(m_scoreText);
    }

} // namespace blast4
