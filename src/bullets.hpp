#ifndef BLAST4_BULLETS_HPP
#define BLAST4_BULLETS_HPP

#include "context.hpp"

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace blast4
{

    struct Bullet
    {
        sf::Vector2f velocity;
        sf::CircleShape shape;
    };

    class Bullets
    {
      public:
        Bullets();

        void setup(Context & context);
        void update(Context & context);
        void draw(Context & context) const;

        void create(
            Context & context, const sf::Vector2f & position, const sf::Vector2f & unit_velocity);

      private:
        std::vector<Bullet> m_bullets;
    };

} // namespace blast4

#endif // BLAST4_BULLETS_HPP
