#include "bullets.hpp"

#include "board.hpp"
#include "settings.hpp"
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

            const bool didHitWall{
                context.board.isCollisionWithBlock(bullet.shape.getGlobalBounds()) ||
                context.board.isCollisionWithBoardEdge(bullet.shape.getGlobalBounds())
            };

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

    void Bullets::create(
        Context & context, const sf::Vector2f & position, const sf::Vector2f & unit_velocity)
    {
        Bullet & bullet = m_bullets.emplace_back();

        bullet.velocity = (unit_velocity * context.settings.bullet_speed);
        bullet.shape.setFillColor(context.settings.bullet_color);
        bullet.shape.setOutlineColor(context.settings.bullet_color);
        bullet.shape.setOutlineThickness(1.0f);
        bullet.shape.setPointCount(10);
        bullet.shape.setPosition(position);

        bullet.shape.setRadius(
            context.settings.bullet_radius_unit_ratio * context.board.unitSize().x);

        util::setOriginToCenter(bullet.shape);
    }

} // namespace blast4
