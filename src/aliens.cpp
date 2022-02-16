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

    bool Alien::move(const float amount)
    {
        if (move_remaining > 0.0f)
        {
            sprite.move(unit_velocity * amount);
            move_remaining -= amount;
            return true;
        }
        else
        {
            return false;
        }
    }

    void Alien::pickNewMoveToTarget(Context & context)
    {
        const sf::Vector2f alienPosition = util::center(sprite);
        const sf::Vector2s laneIndexes = context.board.laneIndexes(alienPosition);

        if (context.random.boolean())
        {
            const std::vector<float> laneLinesHoriz =
                context.board.findLaneLinesOtherThanHoriz(laneIndexes.x);

            const float laneLineHoriz = context.random.from(laneLinesHoriz);

            move_remaining = std::abs(alienPosition.x - laneLineHoriz);

            unit_velocity.y = 0.0f;
            if (laneLineHoriz < alienPosition.x)
            {
                unit_velocity.x = -1.0f;
            }
            else
            {
                unit_velocity.x = 1.0f;
            }
        }
        else
        {
            const std::vector<float> laneLinesVert =
                context.board.findLaneLinesOtherThanVert(laneIndexes.y);

            const float laneLineVert = context.random.from(laneLinesVert);

            move_remaining = std::abs(alienPosition.y - laneLineVert);

            unit_velocity.x = 0.0f;
            if (laneLineVert < alienPosition.y)
            {
                unit_velocity.y = -1.0f;
            }
            else
            {
                unit_velocity.y = 1.0f;
            }
        }
    }

    bool Alien::shoot(Context & context)
    {
        if (time_until_shoot_sec > 0.0f)
        {
            time_until_shoot_sec -= context.frame_time_sec;
            return false;
        }
        else
        {
            time_until_shoot_sec = context.random.fromTo(
                context.settings.alien_shoot_delay_min_sec,
                context.settings.alien_shoot_delay_max_sec);

            const sf::FloatRect alienBoounds = sprite.getGlobalBounds();
            const sf::Vector2f alienPosition = util::center(alienBoounds);
            const sf::Vector2f playerPosition = util::center(context.starship.globalBounds());
            const sf::Vector2f positionDiff = (playerPosition - alienPosition);

            sf::Vector2f unitVelocity{ 0.0f, 0.0f };
            if (std::abs(positionDiff.x) > std::abs(positionDiff.y))
            {
                if (positionDiff.x < 0.0f)
                {
                    unitVelocity.x = -1.0f;
                }
                else
                {
                    unitVelocity.x = 1.0f;
                }
            }
            else
            {
                if (positionDiff.y < 0.0f)
                {
                    unitVelocity.y = -1.0f;
                }
                else
                {
                    unitVelocity.y = 1.0f;
                }
            }

            if (context.bullets.create(context, false, alienBoounds, unitVelocity))
            {
                context.audio.play("alien-shoot");
                return true;
            }
            else
            {
                return false;
            }
        }
    }

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

        for (int i = 0; i < context.settings.starting_alien_count; ++i)
        {
            placeRandom(context);
        }
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

            alien.shoot(context);

            if (alien.move(moveAmount))
            {
                continue;
            }
            else
            {
                alien.pickNewMoveToTarget(context);
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

        alien.time_until_shoot_sec = context.random.fromTo(
            context.settings.alien_shoot_delay_min_sec, context.settings.alien_shoot_delay_max_sec);

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
            if (!alien.is_alive)
            {
                continue;
            }

            if (alien.sprite.getGlobalBounds().intersects(rect))
            {
                return true;
            }
        }

        return false;
    }

    bool Aliens::handleBulletCollisionIf(Context &, const sf::FloatRect& bulletRect)
    {
        for (Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            if (alien.sprite.getGlobalBounds().intersects(bulletRect))
            {
                alien.is_alive = false;
                return true;
            }
        }

        return false;
    }

} // namespace blast4
