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
        AmmoPickup(const sf::Texture & t_texture)
            : is_alive(true)
            , sprite(t_texture)
        {}

        bool is_alive;
        sf::Sprite sprite;
    };

    //

    class Ammo
    {
      public:
        Ammo();

      public:
        void setup(Context & t_context);
        void draw(Context & t_context) const;
        void placeRandom(Context & t_context);
        bool handleCollisionIf(Context & t_context, const sf::FloatRect & t_rect);

      private:
        sf::Texture m_texture;
        std::vector<AmmoPickup> m_pickups;
    };

} // namespace blast4

#endif // BLAST4_AMMO_HPP
