#ifndef BLAST4_SETTINGS_HPP
#define BLAST4_SETTINGS_HPP

#include <SFML/Graphics/Color.hpp>

namespace blast4
{

    struct Settings
    {
        sf::Color background_color = sf::Color(20, 20, 20);
        sf::Color grid_color = sf::Color::White;
        sf::Color ship_color = sf::Color::White;
        float grid_size_ratio = 0.0275f;
        int grid_side_count = 20;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
