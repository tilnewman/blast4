#ifndef BLAST4_ANIMATION_PLAYER_HPP_INCLUDED
#define BLAST4_ANIMATION_PLAYER_HPP_INCLUDED
//
// animation-player.hpp
//
#include "random.hpp"
#include "sfml-defaults.hpp"

#include <filesystem>
#include <memory>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

namespace util
{
    struct AnimConfig
    {
        AnimConfig()
            : AnimConfig(2.0f)
        {
            is_default = true;
        }

        explicit AnimConfig(
            const float t_durationSec,
            const sf::Color t_color = sf::Color::White,
            const sf::BlendMode t_blendMode = sf::BlendAlpha)
            : is_default(false)
            , duration_sec(t_durationSec)
            , color(t_color)
            , blend_mode(t_blendMode)
        {}

        bool is_default;
        float duration_sec;
        sf::Color color;
        sf::BlendMode blend_mode;
    };

    // Animations are simply directories with one or more images that each contain one or more
    // frames.  The images must be named so that simple alpha-numeric sorting works.  Supported
    // image file types are: bmp, jpg, png, and tga.  Directorty names must be formatted
    // this way:   <name>-<width>x<height>
    //
    //  seizure-inducing-flashes-256x512
    //
    // The last number can be skipped if it's the same:
    //  immorally-suggestive-cartoon-128x
    //
    // If multiple animations directories start with the same name...
    //  addictive-carcinogenic-product-advertisement-for-kids-100x
    //  addictive-carcinogenic-product-advertisement-for-teens-100x
    //  addictive-carcinogenic-product-advertisement-for-liberals-100x
    //  addictive-carcinogenic-product-advertisement-for-conservatives-100x
    //  addictive-carcinogenic-product-advertisement-for-seniors-100x
    //
    // ...then one call to play("addictive-carcinogenic-product-advertisement")
    // will pick one for you at random.  You know, if you can't decide since their all legal.
    //
    class AnimationPlayer : public sf::Drawable
    {
        struct ParsedDirectoryName
        {
            std::string name{};
            sf::Vector2i frame_size{};
        };

        struct Image
        {
            std::string filename{};
            sf::Texture texture{};
            std::vector<sf::IntRect> rects{};
        };

        struct ImageCache
        {
            ImageCache(
                const std::size_t t_index,
                const std::string & t_name,
                const AnimConfig & t_config,
                const sf::Vector2f & t_size)
                : config(t_config)
                , index(t_index)
                , frame_size(t_size)
                , animation_name(t_name)
                , frame_count(0)
                , images()
            {}

            std::string toString() const;

            AnimConfig config;
            std::size_t index;
            sf::Vector2f frame_size;
            std::string animation_name;
            std::size_t frame_count;
            std::vector<Image> images;
        };

        struct Animation
        {
            AnimConfig config{};
            bool is_playing{ false };
            sf::Sprite sprite{ util::SfmlDefaults::instance().texture() };
            std::size_t cache_index{ 0 };
            std::size_t frame_index{ 0 };
            float sec_elapsed{ 0.0f };
        };

      public:
        explicit AnimationPlayer(const Random & t_random, const std::string & t_pathStr = {});

        [[nodiscard]] inline std::size_t playingAtOnceMax() const noexcept
        {
            return m_maxPlayingAtOnceCount;
        }

        inline void playingAtOnceMax(const std::size_t t_maxCount) noexcept
        {
            m_maxPlayingAtOnceCount = t_maxCount;
        }

        bool loadAll(const AnimConfig & t_config = {});

        bool load(
            const std::initializer_list<std::string> t_names, const AnimConfig & t_config = {});

        bool load(const std::string & t_name, const AnimConfig & t_config = {});

        void configure(const std::string & t_name, const AnimConfig & t_config);

        void play(
            const std::string & t_name,
            const sf::FloatRect & t_bounds,
            const AnimConfig & t_config = {});

        inline void play(
            const std::string & t_name,
            const sf::Vector2f & t_pos,
            const float t_size,
            const AnimConfig & t_config = {})
        {
            play(t_name, sf::FloatRect(t_pos, sf::Vector2f(t_size, t_size)), t_config);
        }

        void stop(const std::string & t_name);
        void update(const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const override;
        void stopAll();
        void reset();

      private:
        void loadAnimationDirectories(
            const std::string & t_name = "", const AnimConfig & t_config = {});

        bool willLoadAnimationDirectory(
            const std::filesystem::directory_entry & t_dirEntry,
            ParsedDirectoryName & t_parse,
            const std::string & t_name = "") const;

        void loadAnimationDirectory(
            const std::filesystem::directory_entry & t_dirEntry,
            const ParsedDirectoryName & t_parse,
            const AnimConfig & t_config);

        //

        bool loadAnimationImages(
            const std::filesystem::directory_entry & t_dirEntry, ImageCache & t_cache) const;

        bool willLoadAnimationImage(const std::filesystem::directory_entry & t_fileEntry) const;

        bool loadAnimationImage(
            const std::filesystem::directory_entry & t_fileEntry, ImageCache & t_cache) const;

        //

        ParsedDirectoryName parseDirectoryName(const std::string & t_name) const;

        Animation & getAvailableAnimation();

        void updateAnimation(Animation & t_anim, const float t_elapsedTimeSec) const;

        void setAnimationFrame(Animation & t_anim, const std::size_t t_newFrameIndex) const;

        std::vector<std::size_t> findCacheIndexesByName(const std::string & t_name) const;

        void createAnimation(
            const std::vector<std::size_t> & t_possibleCacheIndexes,
            const sf::FloatRect & t_bounds,
            const AnimConfig & t_config);

      private:
        const Random & m_random;
        std::string m_pathStr;
        std::vector<Animation> m_animations;
        std::vector<std::unique_ptr<ImageCache>> m_imageCaches;
        std::string m_fileExtensions;
        std::size_t m_maxPlayingAtOnceCount;
    };
} // namespace util

#endif // BLAST4_ANIMATION_PLAYER_HPP_INCLUDED
