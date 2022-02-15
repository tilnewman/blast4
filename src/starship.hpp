#ifndef BLAST4_STARSHIP_HPP
#define BLAST4_STARSHIP_HPP

#include "context.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace blast4
{

    class Starship
    {
      public:
        Starship();

      public:
        void setup(Context & context);
        void update(Context & context);
        void draw(Context & context) const;
        void handleEvent(Context & context, const sf::Event & event);

        bool intersects(const sf::FloatRect & rect) const
        {
            return m_sprite.getGlobalBounds().intersects(rect);
        }

        const sf::FloatRect globalBounds() const { return m_sprite.getGlobalBounds(); }

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };

} // namespace blast4

#endif // BLAST4_STARSHIP_HPP
