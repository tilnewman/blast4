#ifndef BLAST4_SETTINGS_HPP
#define BLAST4_SETTINGS_HPP

#include <SFML/Graphics/Color.hpp>

namespace blast4
{

    struct Settings
    {
        sf::Color background_color = sf::Color(27, 31, 35);
        sf::Color board_color = sf::Color::Black;
        sf::Color block_color = background_color + sf::Color(100, 100, 120);
        sf::Color ship_color = sf::Color(220, 220, 220);
        sf::Color bullet_color = sf::Color::White;

        float unit_size_ratio = 0.03f;
        float edge_pad_ratio = 0.05f;
        float ship_speed = 125.0f;
        float bullet_speed = 300.0f;
        float bullet_radius_unit_ratio = 0.1f;
        float bullet_shoot_delay_sec = 0.1f;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
