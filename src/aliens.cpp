// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

        for (Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            if (alien.move_remaining > 0.0f)
            {
                alien.sprite.move(alien.unit_velocity * moveAmount);
                alien.move_remaining -= moveAmount;
            }
            else
            {
                const sf::Vector2f alienPosition = util::center(alien.sprite);
                const sf::Vector2s laneIndexes = context.board.laneIndexes(alienPosition);

                if (context.random.boolean())
                {
                    const std::vector<float> laneLinesHoriz =
                        context.board.findLaneLinesOtherThanHoriz(laneIndexes.x);

                    const float laneLineHoriz = context.random.from(laneLinesHoriz);

                    alien.move_remaining = std::abs(alienPosition.x - laneLineHoriz);

                    alien.unit_velocity.y = 0.0f;
                    if (laneLineHoriz < alienPosition.x)
                    {
                        alien.unit_velocity.x = -1.0f;
                    }
                    else
                    {
                        alien.unit_velocity.x = 1.0f;
                    }
                }
                else
                {
                    const std::vector<float> laneLinesVert =
                        context.board.findLaneLinesOtherThanVert(laneIndexes.y);

                    const float laneLineVert = context.random.from(laneLinesVert);

                    alien.move_remaining = std::abs(alienPosition.y - laneLineVert);

                    alien.unit_velocity.x = 0.0f;
                    if (laneLineVert < alienPosition.y)
                    {
                        alien.unit_velocity.y = -1.0f;
                    }
                    else
                    {
                        alien.unit_velocity.y = 1.0f;
                    }
                }
            }
        }

        m_aliens.erase(
            std::remove_if(
                std::begin(m_aliens),
                std::end(m_aliens),
                [](const Alien & alien) { return !alien.is_alive; }),
            std::end(m_aliens));
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
            case 1:  { alien.sprite.setTexture(m_texture1); break; }
            case 2:  { alien.sprite.setTexture(m_texture2); break; }
            case 3:
            default: { alien.sprite.setTexture(m_texture3); break; }
        };
        //clang-format on

        alien.sprite.setColor(context.settings.alien_color);
        util::fit(alien.sprite, (context.board.shipSize() * 0.9f));
        util::setOriginToCenter(alien.sprite);
        
        alien.sprite.setPosition(context.board.randomFreePosition(context));

        m_aliens.push_back(alien);
    }

    bool Aliens::isCollision(const sf::FloatRect & rect) const
    {
        for (const Alien & alien : m_aliens)
        {
            if (alien.sprite.getGlobalBounds().intersects(rect))
            {
                return true;
            }
        }

        return false;
    }

    bool Aliens::handleBulletCollisionIf(Context & context, const sf::FloatRect& bulletRect)
    {
        for (Alien & alien : m_aliens)
        {
            if (alien.sprite.getGlobalBounds().intersects(bulletRect))
            {
                context.audio.play("bullet-hits-alien");
                alien.is_alive = false;
                return true;
            }
        }

        return false;
    }

} // namespace blast4
