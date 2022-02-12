#ifndef BLAST4_SETTINGS_HPP
#define BLAST4_SETTINGS_HPP

#include <SFML/Graphics/Color.hpp>

namespace blast4
{

    struct Settings
    {
        sf::Color background_color = sf::Color(30, 25, 45);
        sf::Color board_color = sf::Color::Black;
        sf::Color block_color = sf::Color(130, 130, 130);
        sf::Color ship_color = sf::Color(220, 220, 220);
        float unit_size_ratio = 0.03f;
        float edge_pad_ratio = 0.05f;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
