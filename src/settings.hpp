#ifndef BLAST4_SETTINGS_HPP
#define BLAST4_SETTINGS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace blast4
{

    struct Settings
    {
        sf::Color background_color = sf::Color(27, 31, 35);
        sf::Color board_color = sf::Color::Black;
        sf::Color block_color = sf::Color(45, 55, 60);
        sf::Color ship_color = sf::Color(220, 220, 220);
        sf::Color bullet_color = sf::Color::White;
        sf::Color title_color = sf::Color(220, 220, 255);
        sf::Color text_color = sf::Color(100, 100, 200);
        sf::Color alien_color = (block_color + sf::Color(50, 50, 50));

        sf::Vector2i block_count = { 9, 4 };

        float alien_shoot_delay_min_sec = 0.5f;
        float alien_shoot_delay_max_sec = 5.0f;

        float ship_size_ratio = 0.03f;
        float edge_pad_ratio = 0.05f;
        float border_pad_ratio = 0.01f;
        float ship_speed = 125.0f;
        float bullet_speed = 375.0f;
        float bullet_radius_ship_ratio = 0.1f;
        float bullet_shoot_delay_sec = 0.1f;
        float text_size_ratio = 0.05f;

        int starting_ammo = 10;
        int score_for_killing_alien = 1;
        int starting_alien_count = 5;
    };

} // namespace blast4

#endif // BLAST4_SETTINGS_HPP
