// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "starship.hpp"

#include "aliens.hpp"
#include "ammo.hpp"
#include "animation-player.hpp"
#include "board.hpp"
#include "bullets.hpp"
#include "check-macros.hpp"
#include "game.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "states.hpp"
#include "texture-loader.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Starship::Starship()
        : m_texture()
        , m_sprite(m_texture)
    {}

    void Starship::setup(Context & t_context)
    {
        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/player-ship.png"));

        m_sprite.setTexture(m_texture, true);
        m_sprite.setColor(t_context.settings.ship_color);
        util::fit(m_sprite, (t_context.board.shipSize() * 0.9f));
        util::setOriginToCenter(m_sprite);
        m_sprite.setPosition(t_context.board.randomFreePosition(t_context));
    }

    void Starship::update(Context & t_context)
    {
        const float moveAmount{ t_context.frame_time_sec * t_context.settings.ship_speed };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::I))
        {
            m_sprite.move({ 0.0f, -moveAmount });

            if (t_context.board.isCollision(m_sprite.getGlobalBounds()))
            {
                m_sprite.move({ 0.0f, moveAmount });
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::M))
        {
            m_sprite.move({ 0.0f, moveAmount });

            if (t_context.board.isCollision(m_sprite.getGlobalBounds()))
            {
                m_sprite.move({ 0.0f, -moveAmount });
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::L))
        {
            m_sprite.move({ moveAmount, 0.0f });

            if (t_context.board.isCollision(m_sprite.getGlobalBounds()))
            {
                m_sprite.move({ -moveAmount, 0.0f });
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::J))
        {
            m_sprite.move({ -moveAmount, 0.0f });

            if (t_context.board.isCollision(m_sprite.getGlobalBounds()))
            {
                m_sprite.move({ moveAmount, 0.0f });
            }
        }

        // after moving, capture position
        const sf::FloatRect bounds{ m_sprite.getGlobalBounds() };

        if (t_context.ammo.handleCollisionIf(t_context, bounds))
        {
            t_context.audio.play("pickup");
            t_context.game.ammo += t_context.settings.ammo_per_pickup;
            t_context.game.score += t_context.settings.score_for_pickup;
            t_context.ammo.placeRandom(t_context);
        }

        if (t_context.aliens.isCollision(bounds))
        {
            t_context.audio.play("bullet-hits-player");
            t_context.states.setChangePending(State::End);

            const sf::Vector2f animPosition{ util::center(bounds) - (bounds.size * 2.0f) };
            const float animSize{ bounds.size.x * 4.0f };
            t_context.anim.play("explode", animPosition, animSize, util::AnimConfig{ 2.0f });
        }
    }

    void Starship::draw(Context & t_context) const { t_context.window.draw(m_sprite); }

    void Starship::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (const auto * keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            const sf::FloatRect shipRect{ m_sprite.getGlobalBounds() };

            bool didShoot = false;
            sf::Vector2f unitVelocity;

            if (sf::Keyboard::Scancode::E == keyPtr->scancode)
            {
                didShoot = true;
                unitVelocity = sf::Vector2f{ 0.0f, -1.0f };
            }
            else if (sf::Keyboard::Scancode::X == keyPtr->scancode)
            {
                didShoot = true;
                unitVelocity = sf::Vector2f{ 0.0f, 1.0f };
            }
            else if (sf::Keyboard::Scancode::S == keyPtr->scancode)
            {
                didShoot = true;
                unitVelocity = sf::Vector2f{ -1.0f, 0.0f };
            }
            else if (sf::Keyboard::Scancode::F == keyPtr->scancode)
            {
                didShoot = true;
                unitVelocity = sf::Vector2f{ 1.0f, 0.0f };
            }

            if (didShoot)
            {
                if (t_context.game.ammo <= 0)
                {
                    t_context.audio.play("no-more-bullets");
                    return;
                }

                if (t_context.bullets.create(t_context, true, shipRect, unitVelocity))
                {
                    t_context.audio.play("player-shoot");
                    --t_context.game.ammo;
                }
                else
                {
                    t_context.audio.play("bullet-hits-block");
                }
            }
        }
    }

} // namespace blast4
