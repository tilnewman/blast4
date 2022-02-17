#ifndef BLAST4_AMMO_HPP
#define BLAST4_AMMO_HPP

#include "context.hpp"

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace blast4
{

    struct AmmoPickup
    {
        bool is_alive = false;
        sf::Sprite sprite;
    };

    class Ammo
    {
      public:
        Ammo();

      public:
        void setup(Context & context);
        void draw(Context & context) const;
        void placeRandom(Context & context);
        bool handleCollisionIf(Context & context, const sf::FloatRect & rect);

      private:
        sf::Texture m_texture;
        std::vector<AmmoPickup> m_pickups;
    };

} // namespace blast4

#endif // BLAST4_AMMO_HPP
