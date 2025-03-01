// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ammo.hpp"

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

    Ammo::Ammo()
        : m_texture{}
        , m_pickups{}
    {}

    void Ammo::setup(Context & t_context)
    {
        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/ammo.png").string());

        for (int i = 0; i < t_context.settings.starting_ammo_pickup_count; ++i)
        {
            placeRandom(t_context);
        }
    }

    void Ammo::draw(Context & t_context) const
    {
        for (const AmmoPickup & pickup : m_pickups)
        {
            if (pickup.is_alive)
            {
                t_context.window.draw(pickup.sprite);
            }
        }
    }

    void Ammo::placeRandom(Context & t_context)
    {
        AmmoPickup pickup(m_texture);
        pickup.sprite.setColor(t_context.settings.ammo_color);
        util::fit(pickup.sprite, (t_context.board.shipSize() * 0.75f));
        util::setOriginToCenter(pickup.sprite);
        pickup.sprite.setPosition(t_context.board.randomFreePosition(t_context));

        m_pickups.push_back(pickup);
    }

    bool Ammo::handleCollisionIf(Context &, const sf::FloatRect & t_bulletRect)
    {
        for (AmmoPickup & pickup : m_pickups)
        {
            if (!pickup.is_alive)
            {
                continue;
            }

            if (pickup.sprite.getGlobalBounds().findIntersection(t_bulletRect))
            {
                pickup.is_alive = false;
                return true;
            }
        }

        return false;
    }

} // namespace blast4
