#ifndef BLAST4_TOP_PANEL_HPP
#define BLAST4_TOP_PANEL_HPP

#include "context.hpp"

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace blast4
{

    class TopPanel
    {
      public:
        TopPanel();

        void setup(Context & context);
        void draw(Context & context) const;

      private:
        sf::Vector2f m_windowSize;
        sf::FloatRect m_boardRect;
        sf::FloatRect m_titleRect;
        sf::FloatRect m_scoreRect;
        sf::FloatRect m_ammoRect;

        sf::Font m_titleFont;
        sf::Text m_titleText;

        sf::Font m_generalFont;
        sf::Text m_ammoText;
        sf::Text m_scoreText;
    };

} // namespace blast4

#endif // BLAST4_TOP_PANEL_HPP
