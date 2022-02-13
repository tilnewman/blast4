#include "images.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "settings.hpp"
#include "util.hpp"

namespace blast4
{

    Images::Images()
        : m_shipTexture()
        , m_shipSprite()
    {}

    void Images::setup(Context & context)
    {
        m_shipTexture.loadFromFile("media/image/ship.png");
        m_shipSprite.setTexture(m_shipTexture);
        m_shipSprite.setColor(context.settings.ship_color);
        util::fit(m_shipSprite, (context.board.unitSize() * 0.9f));
        util::setOriginToCenter(m_shipSprite);
        m_shipSprite.setPosition(context.board.startPosition());
    }

} // namespace blast4
