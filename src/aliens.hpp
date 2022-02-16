#ifndef BLAST4_ALIENS_HPP
#define BLAST4_ALIENS_HPP

#include "context.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace blast4
{

    struct Alien
    {
        bool is_alive = false;
        float move_remaining = 0.0f;
        sf::Vector2f unit_velocity;
        float time_until_shoot_sec = 0.0f;
        sf::Sprite sprite;

        bool move(const float amount);
        bool shoot(Context & context);
        void pickNewMoveToTarget(Context & context);
    };

    class Aliens
    {
      public:
        Aliens();

      public:
        void setup(Context & context);
        void update(Context & context);
        void draw(Context & context) const;
        void placeRandom(Context & context);
        bool isCollision(const sf::FloatRect & rect) const;
        bool handleBulletCollisionIf(Context & context, const sf::FloatRect & bulletRect);

      private:
        sf::Texture m_texture1;
        sf::Texture m_texture2;
        sf::Texture m_texture3;

        std::vector<Alien> m_aliens;
    };

} // namespace blast4

#endif // BLAST4_ALIENS_HPP
