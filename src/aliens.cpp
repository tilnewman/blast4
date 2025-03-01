// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "aliens.hpp"

#include "board.hpp"
#include "bullets.hpp"
#include "check-macros.hpp"
#include "game.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
#include "texture-loader.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    bool Alien::move(const float t_amount)
    {
        if (move_remaining > 0.0f)
        {
            sprite.move(unit_velocity * t_amount);
            move_remaining -= t_amount;
            return true;
        }
        else
        {
            return false;
        }
    }

    void Alien::pickNewMoveToTarget(Context & t_context)
    {
        const sf::Vector2f alienPosition{ util::center(sprite) };
        const sf::Vector2s laneIndexes{ t_context.board.laneIndexes(alienPosition) };

        if (t_context.random.boolean())
        {
            const std::vector<float> laneLinesHoriz{ t_context.board.findLaneLinesOtherThanHoriz(
                laneIndexes.x) };

            const float laneLineHoriz{ t_context.random.from(laneLinesHoriz) };

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
            const std::vector<float> laneLinesVert{ t_context.board.findLaneLinesOtherThanVert(
                laneIndexes.y) };

            const float laneLineVert{ t_context.random.from(laneLinesVert) };

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

    bool Alien::shoot(Context & t_context)
    {
        if (time_until_shoot_sec > 0.0f)
        {
            time_until_shoot_sec -= t_context.frame_time_sec;
            return false;
        }
        else
        {
            time_until_shoot_sec = t_context.random.fromTo(
                t_context.settings.alien_shoot_delay_min_sec,
                t_context.settings.alien_shoot_delay_max_sec);

            const sf::FloatRect alienBoounds{ sprite.getGlobalBounds() };
            const sf::Vector2f alienPosition{ util::center(alienBoounds) };
            const sf::Vector2f playerPosition{ util::center(t_context.starship.globalBounds()) };
            const sf::Vector2f positionDiff{ (playerPosition - alienPosition) };

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

            if (t_context.bullets.create(t_context, false, alienBoounds, unitVelocity))
            {
                t_context.audio.play("alien-shoot");
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //

    Aliens::Aliens()
        : m_texture1{}
        , m_texture2{}
        , m_aliens{}
    {}

    void Aliens::setup(Context & t_context)
    {
        util::TextureLoader::load(
            m_texture1, (t_context.settings.media_path / "image/alien-ship-1.png"));

        util::TextureLoader::load(
            m_texture2, (t_context.settings.media_path / "image/alien-ship-2.png"));

        for (int i = 0; i < t_context.settings.starting_alien_count; ++i)
        {
            placeRandom(t_context);
        }
    }

    void Aliens::update(Context & t_context)
    {
        const float moveAmount{ t_context.frame_time_sec * t_context.settings.ship_speed };

        for (Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            alien.shoot(t_context);

            if (alien.move(moveAmount))
            {
                continue;
            }
            else
            {
                alien.pickNewMoveToTarget(t_context);
            }
        }

        std::erase_if(m_aliens, [](const Alien & alien) { return !alien.is_alive; });
    }

    void Aliens::draw(Context & t_context) const
    {
        for (const Alien & alien : m_aliens)
        {
            if (alien.is_alive)
            {
                t_context.window.draw(alien.sprite);
            }
        }
    }

    void Aliens::placeRandom(Context & t_context)
    {
        Alien alien;
        alien.is_alive = true;

        alien.time_until_shoot_sec = t_context.random.fromTo(
            t_context.settings.alien_shoot_delay_min_sec,
            t_context.settings.alien_shoot_delay_max_sec);

        if (t_context.random.boolean())
        {
            alien.sprite.setTexture(m_texture1, true);
        }
        else
        {
            alien.sprite.setTexture(m_texture2, true);
        }

        alien.sprite.setColor(t_context.settings.alien_color);
        util::fit(alien.sprite, (t_context.board.shipSize() * 1.0f));
        util::setOriginToCenter(alien.sprite);
        alien.sprite.setPosition(t_context.board.randomFreeFarPosition(t_context));

        m_aliens.push_back(alien);
    }

    bool Aliens::isCollision(const sf::FloatRect & t_rect) const
    {
        for (const Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            if (alien.sprite.getGlobalBounds().findIntersection(t_rect))
            {
                return true;
            }
        }

        return false;
    }

    bool Aliens::handleBulletCollisionIf(
        Context &, const sf::FloatRect & t_bulletRect, sf::FloatRect & t_collidingRectOutParam)
    {
        for (Alien & alien : m_aliens)
        {
            if (!alien.is_alive)
            {
                continue;
            }

            if (alien.sprite.getGlobalBounds().findIntersection(t_bulletRect))
            {
                t_collidingRectOutParam = alien.sprite.getGlobalBounds();
                alien.is_alive = false;
                return true;
            }
        }

        return false;
    }

} // namespace blast4
