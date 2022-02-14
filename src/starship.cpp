#include "starship.hpp"

#include "board.hpp"
#include "bullets.hpp"
#include "check-macros.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace blast4
{

    Starship::Starship()
        : m_texture()
        , m_sprite()
    {}

    void Starship::setup(Context & context)
    {
        m_texture.loadFromFile("media/image/player-ship.png");
        m_sprite.setTexture(m_texture);
        m_sprite.setColor(context.settings.ship_color);
        util::fit(m_sprite, (context.board.shipSize() * 0.9f));
        util::setOriginToCenter(m_sprite);
        m_sprite.setPosition(context.board.startPosition());
    }

    void Starship::update(Context & context)
    {
        const float moveAmount{ context.frame_time_sec * context.settings.ship_speed };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            m_sprite.move(0.0f, -moveAmount);

            if (context.board.isCollisionWithBlock(m_sprite.getGlobalBounds()) ||
                context.board.isCollisionWithBoardEdge(m_sprite.getGlobalBounds()))
            {
                m_sprite.move(0.0f, moveAmount);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            m_sprite.move(0.0f, moveAmount);

            if (context.board.isCollisionWithBlock(m_sprite.getGlobalBounds()) ||
                context.board.isCollisionWithBoardEdge(m_sprite.getGlobalBounds()))
            {
                m_sprite.move(0.0f, -moveAmount);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            m_sprite.move(moveAmount, 0.0f);

            if (context.board.isCollisionWithBlock(m_sprite.getGlobalBounds()) ||
                context.board.isCollisionWithBoardEdge(m_sprite.getGlobalBounds()))
            {
                m_sprite.move(-moveAmount, 0.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            m_sprite.move(-moveAmount, 0.0f);

            if (context.board.isCollisionWithBlock(m_sprite.getGlobalBounds()) ||
                context.board.isCollisionWithBoardEdge(m_sprite.getGlobalBounds()))
            {
                m_sprite.move(moveAmount, 0.0f);
            }
        }
    }

    void Starship::draw(Context & context) const { context.window.draw(m_sprite); }

    void Starship::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        const sf::FloatRect shipRect = m_sprite.getGlobalBounds();

        bool didShoot = false;
        sf::Vector2f startPosition;
        sf::Vector2f unitVelocity;

        if (sf::Keyboard::E == event.key.code)
        {
            didShoot = true;
            unitVelocity = sf::Vector2f{ 0.0f, -1.0f };
            startPosition = sf::Vector2f{ (shipRect.left + (shipRect.width * 0.5f)), shipRect.top };
        }
        else if (sf::Keyboard::X == event.key.code)
        {
            didShoot = true;

            unitVelocity = sf::Vector2f{ 0.0f, 1.0f };

            startPosition =
                sf::Vector2f{ (shipRect.left + (shipRect.width * 0.5f)), util::bottom(shipRect) };
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            didShoot = true;

            unitVelocity = sf::Vector2f{ -1.0f, 0.0f };

            startPosition =
                sf::Vector2f{ shipRect.left, (shipRect.top + (shipRect.height * 0.5f)) };
        }
        else if (sf::Keyboard::F == event.key.code)
        {
            didShoot = true;

            unitVelocity = sf::Vector2f{ 1.0f, 0.0f };

            startPosition =
                sf::Vector2f{ util::right(shipRect), (shipRect.top + (shipRect.height * 0.5f)) };
        }

        if (didShoot)
        {
            if (context.bullets.create(context, shipRect, startPosition, unitVelocity))
            {
                context.audio.play("player-shoot");
            }
            else
            {
                context.audio.play("bullet-hits-block");
            }
        }
    }

} // namespace blast4
