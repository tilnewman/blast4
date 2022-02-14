#include "bullets.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "starship.hpp"
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

            if (context.board.isCollisionWithBoardEdge(bullet.shape.getGlobalBounds()))
            {
                context.audio.play("bullet-hits-wall");
                bullet.is_alive = false;
            }
        }

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
                    context.audio.play("bullet-hits-bullet");
                    bulletOuter.is_alive = false;
                    bulletInner.is_alive = false;
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
        const sf::FloatRect & shooterBounds,
        const sf::Vector2f & position,
        const sf::Vector2f & unit_velocity)
    {
        Bullet bullet;

        bullet.is_alive = true;
        bullet.velocity = (unit_velocity * context.settings.bullet_speed);
        bullet.shape.setFillColor(context.settings.bullet_color);
        bullet.shape.setOutlineColor(context.settings.bullet_color);
        bullet.shape.setOutlineThickness(0.0f);
        bullet.shape.setPointCount(10);

        const float radius =
            (context.settings.bullet_radius_unit_ratio * context.board.unitSize().x);

        bullet.shape.setRadius(radius);

        util::setOriginToCenter(bullet.shape);

        // move bullet position far enough away from the starship sprite that fired it
        // so that it doesn't hit the starship that fired it
        bullet.shape.setPosition({ position + (unit_velocity * (radius * 1.5f)) });

        const sf::FloatRect bulletBounds = bullet.shape.getGlobalBounds();

        if (shooterBounds.intersects(bulletBounds) ||
            context.board.isCollisionWithBlock(bulletBounds))
        {
            context.audio.play("bullet-hits-block");
            return false;
        }
        else
        {
            m_bullets.push_back(bullet);
            return true;
        }
    }

} // namespace blast4
