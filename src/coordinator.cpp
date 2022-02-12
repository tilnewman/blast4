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
        const sf::Vector2f shipPosition = m_images.shipSprite().getPosition();
        const sf::FloatRect shipBounds = m_images.shipSprite().getGlobalBounds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            const float lane = m_board.findLaneHoriz(shipPosition.x);
            if ((lane > 0.0f) && (shipBounds.top > m_board.rect().top))
            {
                m_images.shipSprite().setPosition(
                    lane, m_images.shipSprite().getPosition().y - 1.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            const float lane = m_board.findLaneHoriz(shipPosition.x);
            if ((lane > 0.0f) && (util::bottom(shipBounds) < util::bottom(m_board.rect())))
            {
                m_images.shipSprite().setPosition(
                    lane, m_images.shipSprite().getPosition().y + 1.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            const float lane = m_board.findLaneVert(shipPosition.y);
            if ((lane > 0.0f) && (util::right(shipBounds) < util::right(m_board.rect())))
            {
                m_images.shipSprite().setPosition(
                    m_images.shipSprite().getPosition().x + 1.0f, lane);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            const float lane = m_board.findLaneVert(shipPosition.y);
            if ((lane > 0.0f) && (shipBounds.left > m_board.rect().left))
            {
                m_images.shipSprite().setPosition(
                    m_images.shipSprite().getPosition().x - 1.0f, lane);
            }
        }
    }

} // namespace blast4
