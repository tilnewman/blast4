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
        bool is_alive = false;
        sf::Vector2f velocity;
        sf::CircleShape shape;
        bool is_from_player = false;
    };

    class Bullets
    {
      public:
        Bullets();

        void setup(Context & context);
        void update(Context & context);
        void draw(Context & context) const;

        bool create(
            Context & context,
            const bool isFromPlayer,
            const sf::FloatRect & shooterBounds,
            const sf::Vector2f & unit_velocity);

      private:
        std::vector<Bullet> m_bullets;
    };

} // namespace blast4

#endif // BLAST4_BULLETS_HPP
