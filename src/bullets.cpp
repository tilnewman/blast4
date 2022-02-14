#include "bullets.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "settings.hpp"
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

        auto iter = m_bullets.begin();
        while (iter != m_bullets.end())
        {
            const Bullet & bullet = *iter;

            const bool didHitWall{ context.board.isCollisionWithBoardEdge(
                bullet.shape.getGlobalBounds()) };

            if (didHitWall)
            {
                iter = m_bullets.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    void Bullets::draw(Context & context) const
    {
        for (const Bullet & bullet : m_bullets)
        {
            context.window.draw(bullet.shape);
        }
    }

    bool Bullets::create(
        Context & context, const sf::Vector2f & position, const sf::Vector2f & unit_velocity)
    {
        Bullet bullet;

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

        const sf::FloatRect globalBounds = bullet.shape.getGlobalBounds();
        M_CHECK(
            (!context.starship.intersects(globalBounds)),
            "Error:  Bullet fired by starship hit that starship!");

        if (context.board.isCollisionWithBlock(globalBounds))
        {
            // TODO sfx reject shot
            return false;
        }
        else
        {
            m_bullets.push_back(bullet);
            return true;
        }
    }

} // namespace blast4
