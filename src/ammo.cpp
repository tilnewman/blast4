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
        : m_texture()
        , m_pickups()
    {}

    void Ammo::setup(Context & context)
    {
        util::TextureLoader::load(
            m_texture, (context.settings.media_path / "image/ammo.png").string());

        for (int i = 0; i < context.settings.starting_ammo_pickup_count; ++i)
        {
            placeRandom(context);
        }
    }

    void Ammo::draw(Context & context) const
    {
        for (const AmmoPickup & pickup : m_pickups)
        {
            if (!pickup.is_alive)
            {
                continue;
            }

            context.window.draw(pickup.sprite);
        }
    }

    void Ammo::placeRandom(Context & context)
    {
        AmmoPickup pickup(m_texture);
        pickup.sprite.setColor(context.settings.ammo_color);
        util::fit(pickup.sprite, (context.board.shipSize() * 0.75f));
        util::setOriginToCenter(pickup.sprite);
        pickup.sprite.setPosition(context.board.randomFreePosition(context));

        m_pickups.push_back(pickup);
    }

    bool Ammo::handleCollisionIf(Context &, const sf::FloatRect & bulletRect)
    {
        for (AmmoPickup & pickup : m_pickups)
        {
            if (!pickup.is_alive)
            {
                continue;
            }

            if (pickup.sprite.getGlobalBounds().findIntersection(bulletRect))
            {
                pickup.is_alive = false;
                return true;
            }
        }

        return false;
    }

} // namespace blast4
