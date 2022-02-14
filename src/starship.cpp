#include "starship.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "settings.hpp"
#include "util.hpp"

namespace blast4
{

    Starship::Starship()
        : m_texture()
        , m_sprite()
    {}

    void Starship::setup(Context & context)
    {
        m_texture.loadFromFile("media/image/ship.png");
        m_sprite.setTexture(m_texture);
        m_sprite.setColor(context.settings.ship_color);
        util::fit(m_sprite, (context.board.unitSize() * 0.9f));
        util::setOriginToCenter(m_sprite);
        m_sprite.setPosition(context.board.startPosition());
    }

} // namespace blast4
