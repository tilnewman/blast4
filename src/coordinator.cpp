#include "coordinator.hpp"

#include "check-macros.hpp"
#include "util.hpp"

namespace blast4
{

    Coordinator::Coordinator(const Settings & settings)
        : m_window()
        , m_settings(settings)
        , m_board()
        , m_images()
        , m_bullets()
        , m_context(m_window, m_settings, m_board, m_images, m_bullets)
    {}

    void Coordinator::play()
    {
        setup();
        loop();
    }

    void Coordinator::setup()
    {
        m_window.create(sf::VideoMode::getDesktopMode(), "Blast4", sf::Style::Fullscreen);
        M_CHECK(m_window.isOpen(), "Error:  Failed to open graphics window!");

        m_board.setup(m_context);
        m_images.setup(m_context);
        m_bullets.setup(m_context);
    }

    void Coordinator::loop()
    {
        sf::Clock frameClock;

        while (m_window.isOpen())
        {
            m_context.frame_time_sec = frameClock.restart().asSeconds();
            handleEvents();
            update();
            draw();
        }
    }

    void Coordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void Coordinator::handleEvent(const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
        }
        else if (sf::Keyboard::E == event.key.code)
        {
            const sf::FloatRect shipRect = m_images.shipSprite().getGlobalBounds();

            const sf::Vector2f startPosition{ (shipRect.left + (shipRect.width * 0.5f)),
                                              shipRect.top };

            m_bullets.create(m_context, startPosition, { 0.0f, -1.0f });
        }
        else if (sf::Keyboard::X == event.key.code)
        {
            const sf::FloatRect shipRect = m_images.shipSprite().getGlobalBounds();

            const sf::Vector2f startPosition{ (shipRect.left + (shipRect.width * 0.5f)),
                                              util::bottom(shipRect) };

            m_bullets.create(m_context, startPosition, { 0.0f, 1.0f });
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            const sf::FloatRect shipRect = m_images.shipSprite().getGlobalBounds();

            const sf::Vector2f startPosition{ shipRect.left,
                                              (shipRect.top + (shipRect.height * 0.5f)) };

            m_bullets.create(m_context, startPosition, { -1.0f, 0.0f });
        }
        else if (sf::Keyboard::F == event.key.code)
        {
            const sf::FloatRect shipRect = m_images.shipSprite().getGlobalBounds();

            const sf::Vector2f startPosition{ util::right(shipRect),
                                              (shipRect.top + (shipRect.height * 0.5f)) };

            m_bullets.create(m_context, startPosition, { 1.0f, 0.0f });
        }
    }

    void Coordinator::update()
    {
        moveShip();
        m_bullets.update(m_context);
    }

    void Coordinator::draw()
    {
        m_window.clear(m_context.settings.background_color);

        m_board.draw(m_context);
        m_window.draw(m_context.images.shipSprite());
        m_bullets.draw(m_context);

        m_window.display();
    }

    void Coordinator::moveShip()
    {
        const float moveAmount{ m_context.frame_time_sec * m_settings.ship_speed };

        sf::Sprite & sprite = m_images.shipSprite();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            sprite.move(0.0f, -moveAmount);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(0.0f, moveAmount);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            sprite.move(0.0f, moveAmount);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(0.0f, -moveAmount);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            sprite.move(moveAmount, 0.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(-moveAmount, 0.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            sprite.move(-moveAmount, 0.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(moveAmount, 0.0f);
            }
        }
    }

} // namespace blast4
