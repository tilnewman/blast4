#ifndef BLAST4_STARSHIP_HPP
#define BLAST4_STARSHIP_HPP

#include "context.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace blast4
{

    class Starship
    {
      public:
        Starship();

      public:
        void setup(Context & context);
        sf::Sprite & sprite() { return m_sprite; }

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };

} // namespace blast4

#endif // BLAST4_STARSHIP_HPP
