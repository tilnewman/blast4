#ifndef BLAST4_ALIENS_HPP
#define BLAST4_ALIENS_HPP

#include "context.hpp"
#include "sfml-defaults.hpp"

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace blast4
{

    struct Alien
    {
        bool move(const float t_amount);
        bool shoot(Context & t_context);
        void pickNewMoveToTarget(Context & t_context);

        bool is_alive{ false };
        float move_remaining{ 0.0f };
        sf::Vector2f unit_velocity{};
        float time_until_shoot_sec{ 0.0f };
        sf::Sprite sprite{ util::SfmlDefaults::instance().texture() };
    };

    //

    class Aliens
    {
      public:
        Aliens();

      public:
        void setup(Context & t_context);
        void update(Context & t_context);
        void draw(Context & t_context) const;
        void placeRandom(Context & t_context);
        bool isCollision(const sf::FloatRect & t_rect) const;

        bool handleBulletCollisionIf(
            Context & t_context,
            const sf::FloatRect & t_bulletRect,
            sf::FloatRect & t_collidingRectOutParam);

      private:
        sf::Texture m_texture1;
        sf::Texture m_texture2;
        std::vector<Alien> m_aliens;
    };

} // namespace blast4

#endif // BLAST4_ALIENS_HPP
