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

        void setup(Context & t_context);
        void update(Context & t_context);
        void draw(Context & t_context) const;
        void handleEvent(Context & t_context, const sf::Event & t_event);

        [[nodiscard]] inline bool intersects(const sf::FloatRect & t_rect) const
        {
            return m_sprite.getGlobalBounds().findIntersection(t_rect).has_value();
        }

        [[nodiscard]] inline sf::FloatRect globalBounds() const noexcept
        {
            return m_sprite.getGlobalBounds();
        }

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };

} // namespace blast4

#endif // BLAST4_STARSHIP_HPP
