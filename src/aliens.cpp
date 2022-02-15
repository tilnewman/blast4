#include "aliens.hpp"

#include "board.hpp"
#include "bullets.hpp"
#include "check-macros.hpp"
#include "game.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Aliens::Aliens()
        : m_texture1()
        , m_texture2()
        , m_texture3()
        , m_aliens()
    {}

    void Aliens::setup(Context & context)
    {
        m_texture1.loadFromFile("media/image/alien-ship-1.png");
        m_texture2.loadFromFile("media/image/alien-ship-2.png");
        m_texture3.loadFromFile("media/image/alien-ship-3.png");

        // TODO remove after testing
        placeRandom(context);
    }

    void Aliens::update(Context & context)
    {
        const float moveAmount{ context.frame_time_sec * context.settings.ship_speed };

        const sf::Vector2f starshipPosition = util::center(context.starship.globalBounds());

        for (Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            const sf::Vector2f alienPosition = util::center(alien.sprite);

            if (starshipPosition.x < alienPosition.x)
            {
                alien.sprite.move(-moveAmount, 0.0f);
                if (context.board.isCollision(alien.sprite.getGlobalBounds()))
                {
                    alien.sprite.move(moveAmount, 0.0f);
                }
            }
            else if (starshipPosition.x > alienPosition.x)
            {
                alien.sprite.move(moveAmount, 0.0f);
                if (context.board.isCollision(alien.sprite.getGlobalBounds()))
                {
                    alien.sprite.move(-moveAmount, 0.0f);
                }
            }

            if (starshipPosition.y < alienPosition.y)
            {
                alien.sprite.move(0.0f, -moveAmount);
                if (context.board.isCollision(alien.sprite.getGlobalBounds()))
                {
                    alien.sprite.move(0.0f, moveAmount);
                }
            }
            else if (starshipPosition.y > alienPosition.y)
            {
                alien.sprite.move(0.0f, moveAmount);
                if (context.board.isCollision(alien.sprite.getGlobalBounds()))
                {
                    alien.sprite.move(0.0f, -moveAmount);
                }
            }
        }
    }

    void Aliens::draw(Context & context) const
    {
        for (const Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            context.window.draw(alien.sprite);
        }
    }

    void Aliens::placeRandom(Context & context)
    {
        Alien alien;
        alien.is_alive = true;

        // clang-format off
        switch (context.random.fromTo(1, 3))
        {
            case 1:  { alien.sprite.setTexture(m_texture1); }
            case 2:  { alien.sprite.setTexture(m_texture2); }
            case 3:
            default: { alien.sprite.setTexture(m_texture3); }
        };
        //clang-format on

        alien.sprite.setColor(context.settings.alien_color);
        util::fit(alien.sprite, (context.board.shipSize() * 0.9f));
        util::setOriginToCenter(alien.sprite);
        
        alien.sprite.setPosition(context.board.randomPosition(context));

        m_aliens.push_back(alien);
    }


} // namespace blast4
