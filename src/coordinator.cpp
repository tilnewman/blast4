#include "coordinator.hpp"

#include "check-macros.hpp"
#include "util.hpp"

#include <SFML/Window/Event.hpp>

namespace blast4
{

    Coordinator::Coordinator(const Settings & settings)
        : m_window()
        , m_settings(settings)
        , m_board()
        , m_images()
        , m_context(m_window, m_settings, m_board, m_images)
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
    }

    void Coordinator::loop()
    {
        while (m_window.isOpen())
        {
            handleEvents();
            moveShip();
            draw();
        }
    }

    void Coordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::Escape == event.key.code)
                {
                    m_window.close();
                }
            }
        }
    }

    void Coordinator::draw()
    {
        m_window.clear(m_context.settings.background_color);

        m_board.draw(m_context);
        m_window.draw(m_context.images.shipSprite());

        m_window.display();
    }

    void Coordinator::moveShip()
    {
        sf::Sprite & sprite = m_images.shipSprite();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            sprite.move(0.0f, -1.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(0.0f, 1.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            sprite.move(0.0f, 1.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(0.0f, -1.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            sprite.move(1.0f, 0.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(-1.0f, 0.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            sprite.move(-1.0f, 0.0f);

            if (m_board.isCollisionWithBlock(sprite.getGlobalBounds()) ||
                m_board.isCollisionWithBoardEdge(sprite.getGlobalBounds()))
            {
                sprite.move(1.0f, 0.0f);
            }
        }
    }

} // namespace blast4
