// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "bullets.hpp"

#include "aliens.hpp"
#include "animation-player.hpp"
#include "board.hpp"
#include "check-macros.hpp"
#include "game.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
#include "states.hpp"
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Bullets::Bullets()
        : m_bullets()
    {}

    void Bullets::setup(Context &) {}

    void Bullets::update(Context & context)
    {
        for (Bullet & bullet : m_bullets)
        {
            bullet.shape.move(bullet.velocity * context.frame_time_sec);
        }

        for (Bullet & bullet : m_bullets)
        {
            if (!bullet.is_alive)
            {
                continue;
            }

            const sf::FloatRect bulletBounds = bullet.shape.getGlobalBounds();

            if (context.starship.intersects(bulletBounds))
            {
                context.audio.play("bullet-hits-player");

                const sf::FloatRect shipBounds = context.starship.globalBounds();

                if (context.game.ammo <= 0)
                {
                    context.states.setChangePending(State::End);

                    const sf::Vector2f animPosition{ util::center(shipBounds) -
                                                     (util::size(shipBounds) * 2.0f) };

                    const float animSize{ util::size(shipBounds).x * 4.0f };

                    context.anim.play("explode", animPosition, animSize, util::AnimConfig{ 2.0f });
                }
                else
                {
                    context.game.ammo = context.game.ammo / 2;

                    const sf::Vector2f animPosition{ util::center(shipBounds) -
                                                     (util::size(shipBounds) * 0.65f) };

                    const float animSize{ shipBounds.width * 1.3f };

                    context.anim.play(
                        "orbcharge",
                        animPosition,
                        animSize,
                        util::AnimConfig{ 0.2f, sf::Color(0, 255, 255, 150) });
                }

                bullet.is_alive = false;
                return;
            }

            if (context.board.isCollisionWithBoardEdge(bulletBounds))
            {
                context.audio.play("bullet-hits-wall");
                bullet.is_alive = false;
                continue;
            }

            sf::FloatRect alienShipRect;
            if (context.aliens.handleBulletCollisionIf(context, bulletBounds, alienShipRect))
            {
                context.audio.play("bullet-hits-alien");
                bullet.is_alive = false;

                if (bullet.is_from_player)
                {
                    context.game.score += context.settings.score_for_killing_alien;
                }

                context.aliens.placeRandom(context);

                sf::Vector2f animPosition{ util::center(alienShipRect) -
                                           (util::size(alienShipRect) * 2.0f) };

                const float animSize{ alienShipRect.width * 4.0f };
                animPosition.y -= util::size(alienShipRect).y;
                context.anim.play("explode", animPosition, animSize, util::AnimConfig{ 2.0f });

                continue;
            }
        }

        // handle bullets colliding with other bullets
        for (std::size_t outer = 0; outer < m_bullets.size(); ++outer)
        {
            Bullet & bulletOuter = m_bullets.at(outer);
            if (!bulletOuter.is_alive)
            {
                continue;
            }

            for (std::size_t inner = (outer + 1); inner < m_bullets.size(); ++inner)
            {
                Bullet & bulletInner = m_bullets.at(inner);
                if (!bulletInner.is_alive)
                {
                    continue;
                }

                if (bulletOuter.shape.getGlobalBounds().intersects(
                        bulletInner.shape.getGlobalBounds()))
                {
                    bulletOuter.is_alive = false;
                    bulletInner.is_alive = false;

                    context.audio.play("bullet-hits-bullet");

                    const sf::Vector2f animPosition{
                        util::center(bulletInner.shape.getGlobalBounds()) -
                        (util::size(context.starship.globalBounds()) * 0.5f)
                    };

                    const float animSize{ util::size(context.starship.globalBounds()).x * 1.0f };

                    context.anim.play(
                        "lightningball",
                        animPosition,
                        animSize,
                        util::AnimConfig{ 0.5f,
                                          context.settings.bullet_color + sf::Color(75, 75, 75) });
                }
            }
        }

        m_bullets.erase(
            std::remove_if(
                std::begin(m_bullets),
                std::end(m_bullets),
                [](const Bullet & b) { return !b.is_alive; }),
            std::end(m_bullets));
    }

    void Bullets::draw(Context & context) const
    {
        for (const Bullet & bullet : m_bullets)
        {
            context.window.draw(bullet.shape);
        }
    }

    bool Bullets::create(
        Context & context,
        const bool isFromPlayer,
        const sf::FloatRect & shipBounds,
        const sf::Vector2f & unit_velocity)
    {
        Bullet bullet;

        bullet.is_alive = true;
        bullet.is_from_player = isFromPlayer;
        bullet.velocity = (unit_velocity * context.settings.bullet_speed);
        bullet.shape.setFillColor(context.settings.bullet_color);
        bullet.shape.setOutlineColor(context.settings.bullet_color);
        bullet.shape.setOutlineThickness(0.0f);
        bullet.shape.setPointCount(10);

        const float radius =
            (context.settings.bullet_radius_ship_ratio * context.board.shipSize().x);

        bullet.shape.setRadius(radius);

        util::setOriginToCenter(bullet.shape);

        sf::Vector2f startPosition;
        if (unit_velocity.y < 0.0f)
        {
            startPosition =
                sf::Vector2f{ (shipBounds.left + (shipBounds.width * 0.5f)), shipBounds.top };
        }
        else if (unit_velocity.y > 0.0f)
        {
            startPosition = sf::Vector2f{ (shipBounds.left + (shipBounds.width * 0.5f)),
                                          util::bottom(shipBounds) };
        }
        else if (unit_velocity.x < 0.0f)
        {
            startPosition =
                sf::Vector2f{ shipBounds.left, (shipBounds.top + (shipBounds.height * 0.5f)) };
        }
        else if (unit_velocity.x > 0.0f)
        {
            startPosition = sf::Vector2f{ util::right(shipBounds),
                                          (shipBounds.top + (shipBounds.height * 0.5f)) };
        }

        // move bullet position far enough away from the ship sprite that fired it
        // so that it doesn't hit the ship that fired it
        bullet.shape.setPosition({ startPosition + (unit_velocity * (radius * 1.5f)) });

        const sf::FloatRect bulletBounds = bullet.shape.getGlobalBounds();

        if (shipBounds.intersects(bulletBounds) ||
            context.board.isCollisionWithBlock(bulletBounds) ||
            context.board.isCollisionWithBoardEdge(bulletBounds))
        {
            return false;
        }
        else
        {
            m_bullets.push_back(bullet);
            return true;
        }
    }

} // namespace blast4
