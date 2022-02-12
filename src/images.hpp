#ifndef BLAST4_IMAGES_HPP
#define BLAST4_IMAGES_HPP

#include "context.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace blast4
{

    class Images
    {
      public:
        Images();

      public:
        void setup(Context & context);
        sf::Sprite & shipSprite() { return m_shipSprite; }

      private:
        sf::Texture m_shipTexture;
        sf::Sprite m_shipSprite;
    };

} // namespace blast4

#endif // BLAST4_IMAGES_HPP
