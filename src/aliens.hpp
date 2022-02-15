#ifndef BLAST4_ALIENS_HPP
#define BLAST4_ALIENS_HPP

#include "context.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace blast4
{

    struct Alien
    {
        sf::Sprite sprite;
        sf::Clock shoot_clock;
        bool is_alive = false;
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

      private:
        sf::Texture m_texture1;
        sf::Texture m_texture2;
        sf::Texture m_texture3;

        std::vector<Alien> m_aliens;
        Alien m_deadAlien;
    };

} // namespace blast4

#endif // BLAST4_ALIENS_HPP
