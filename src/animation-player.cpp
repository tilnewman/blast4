// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// animation-player.cpp
//
#include "animation-player.hpp"

#include "sfml-util.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace util
{
    AnimationPlayer::AnimationPlayer(const Random & t_random, const std::string & t_pathStr)
        : m_random(t_random)
        , m_pathStr(t_pathStr)
        , m_animations()
        , m_imageCaches()
        , m_fileExtensions(".bmp/.jpg/.jpeg/.png/.tga")
        , m_maxPlayingAtOnceCount(100)
    {}

    void AnimationPlayer::stop(const std::string & t_name)
    {
        const std::vector<std::size_t> indexes{ findCacheIndexesByName(t_name) };

        for (Animation & anim : m_animations)
        {
            if (std::find(std::begin(indexes), std::end(indexes), anim.cache_index) !=
                std::end(indexes))
            {
                anim.is_playing = false;
            }
        }
    }

    void AnimationPlayer::reset()
    {
        stopAll();
        m_animations.clear();
        m_imageCaches.clear();
    }

    bool AnimationPlayer::loadAll(const AnimConfig & t_config)
    {
        reset();
        loadAnimationDirectories("", t_config);
        return !m_imageCaches.empty();
    }

    bool AnimationPlayer::load(
        const std::initializer_list<std::string> t_names, const AnimConfig & t_config)
    {
        bool success{ true };

        for (const std::string & name : t_names)
        {
            if (!load(name, t_config))
            {
                success = false;
            }
        }

        return success;
    }

    bool AnimationPlayer::load(const std::string & t_name, const AnimConfig & t_config)
    {
        if (t_name.empty())
        {
            return false;
        }

        stop(t_name);

        loadAnimationDirectories(t_name, t_config);

        return !findCacheIndexesByName(t_name).empty();
    }

    void AnimationPlayer::configure(const std::string & t_name, const AnimConfig & t_config)
    {
        for (const std::size_t index : findCacheIndexesByName(t_name))
        {
            m_imageCaches.at(index)->config = t_config;
        }
    }

    void AnimationPlayer::stopAll()
    {
        for (Animation & anim : m_animations)
        {
            anim.is_playing = false;
        }
    }

    void AnimationPlayer::play(
        const std::string & t_name, const sf::FloatRect & t_bounds, const AnimConfig & t_config)
    {
        if (t_name.empty())
        {
            std::cerr << "AnimationPlayer::play() called with an empty name." << std::endl;
            return;
        }

        if ((t_bounds.size.x < 1.0f) || (t_bounds.size.y < 1.0f))
        {
            std::cerr << "AnimationPlayer::play(bounds=" << t_bounds
                      << ") called with bounds of sizes less than one." << std::endl;
            return;
        }

        std::vector<std::size_t> nameMatchingIndexes{ findCacheIndexesByName(t_name) };
        if (nameMatchingIndexes.empty())
        {
            std::cout << "AnimationPlayer::play(\"" << t_name
                      << "\") called, but none had that name...";

            if (!load(t_name, t_config))
            {
                std::cout << "AND none were found to load either.  So nothing will happen."
                          << std::endl;

                return;
            }

            nameMatchingIndexes = findCacheIndexesByName(t_name);
            if (nameMatchingIndexes.empty())
            {
                std::cout << "AND even though some anims with that name were loaded, something "
                             "else went wrong away.  Go figure.  So nothing will happen."
                          << std::endl;

                return;
            }

            std::cout << "BUT was able to find and load it.  So it's gonna play now." << std::endl;
        }

        createAnimation(nameMatchingIndexes, t_bounds, t_config);
    }

    void AnimationPlayer::update(const float t_elapsedTimeSec)
    {
        for (Animation & anim : m_animations)
        {
            updateAnimation(anim, t_elapsedTimeSec);
        }
    }

    void AnimationPlayer::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const Animation & anim : m_animations)
        {
            if (anim.is_playing)
            {
                const auto blendModeOrig{ t_states.blendMode };
                t_states.blendMode = anim.config.blend_mode;
                t_target.draw(anim.sprite, t_states);
                t_states.blendMode = blendModeOrig;
            }
        }
    }

    void AnimationPlayer::loadAnimationDirectories(
        const std::string & t_nameToLoad, const AnimConfig & t_config)
    {
        std::filesystem::path path{ m_pathStr };
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        {
            path = std::filesystem::current_path();
        }

        std::filesystem::recursive_directory_iterator dirIter{
            path, std::filesystem::directory_options::skip_permission_denied
        };

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            ParsedDirectoryName parse;

            if (willLoadAnimationDirectory(entry, parse, t_nameToLoad))
            {
                loadAnimationDirectory(entry, parse, t_config);
            }
        }

        if (m_imageCaches.empty())
        {
            std::cerr
                << "AnimationPlayer Error:  No valid animation directories were found.  Supported "
                   "image file types: "
                << m_fileExtensions << std::endl;
        }
    }

    bool AnimationPlayer::willLoadAnimationDirectory(
        const std::filesystem::directory_entry & t_dirEntry,
        ParsedDirectoryName & t_parse,
        const std::string & t_nameToLoad) const
    {
        t_parse = parseDirectoryName(t_dirEntry.path().filename().string());

        const bool isDirNameValid{ !t_parse.name.empty() && (t_parse.frame_size.x > 0) &&
                                   (t_parse.frame_size.y > 0) };

        if (!isDirNameValid)
        {
            return false;
        }

        if (!t_nameToLoad.empty())
        {
            const bool dirNameStartsWith{ t_parse.name.find(t_nameToLoad, 0) == 0 };
            if (dirNameStartsWith)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    void AnimationPlayer::loadAnimationDirectory(
        const std::filesystem::directory_entry & t_dirEntry,
        const ParsedDirectoryName & t_parse,
        const AnimConfig & t_config)
    {
        auto imageCache{ std::make_unique<ImageCache>(
            m_imageCaches.size(), t_parse.name, t_config, sf::Vector2f(t_parse.frame_size)) };

        if (loadAnimationImages(t_dirEntry, *imageCache))
        {
            // std::cout << "Loaded Animation: " << imageCache->toString() << std::endl;
            m_imageCaches.push_back(std::move(imageCache));
        }
    }

    bool AnimationPlayer::loadAnimationImages(
        const std::filesystem::directory_entry & t_dirEntry, ImageCache & t_cache) const
    {
        std::filesystem::directory_iterator dirIter{
            t_dirEntry.path(), std::filesystem::directory_options::skip_permission_denied
        };

        for (const std::filesystem::directory_entry & fileEntry : dirIter)
        {
            if (willLoadAnimationImage(fileEntry))
            {
                if (!loadAnimationImage(fileEntry, t_cache))
                {
                    return false;
                }
            }
        }

        if (t_cache.images.empty())
        {
            std::cerr
                << "AnimationPlayer Error:  Found a directory that is named like an animation "
                   "directory here: \""
                << t_dirEntry.path().string() << "\", but was unable to load any images from it."
                << std::endl;

            return false;
        }

        t_cache.frame_count = 0;
        for (const Image & image : t_cache.images)
        {
            t_cache.frame_count += image.rects.size();
        }

        if (0 == t_cache.frame_count)
        {
            std::cerr
                << "AnimationPlayer Error:  Found a directory that is named like an animation "
                   "directory: \""
                << t_dirEntry.path().string()
                << "\", and was unable to load images from it, but somehow the frame count was "
                   "still zero."
                << std::endl;

            return false;
        }

        // directory iterators do not always go in alphanumeric order, so sort here just in case
        std::ranges::sort(t_cache.images, [](const Image & left, const Image & right) {
            return (left.filename < right.filename);
        });

        return true;
    }

    bool AnimationPlayer::willLoadAnimationImage(
        const std::filesystem::directory_entry & t_fileEntry) const
    {
        if (!t_fileEntry.is_regular_file())
        {
            return false;
        }

        const std::string fileName(t_fileEntry.path().filename().string());
        if (fileName.empty())
        {
            return false;
        }

        const std::string fileExt(t_fileEntry.path().filename().extension().string());
        if ((fileExt.size() != 4) && (fileExt.size() != 5))
        {
            return false;
        }

        return (m_fileExtensions.find(fileExt) < m_fileExtensions.size());
    }

    bool AnimationPlayer::loadAnimationImage(
        const std::filesystem::directory_entry & t_fileEntry, ImageCache & t_cache) const
    {
        const sf::Vector2i frameSize{ t_cache.frame_size };

        Image image;

        if (!image.texture.loadFromFile(t_fileEntry.path().string()))
        {
            std::cerr << "AnimationPlayer Error:  Found a supported file: \""
                      << t_fileEntry.path().string()
                      << "\", but an error occurred while loading it."
                      << std::endl;

            return false;
        }

        image.texture.setSmooth(true);

        const sf::Vector2i imageSize{ image.texture.getSize() };

        for (int vert(0); vert < imageSize.y; vert += frameSize.y)
        {
            for (int horiz(0); horiz < imageSize.x; horiz += frameSize.x)
            {
                image.rects.push_back({ sf::Vector2i(horiz, vert), frameSize });
            }
        }

        if (image.rects.empty())
        {
            std::cerr << "AnimationPlayer Error:  Found a supported file: \""
                      << t_fileEntry.path().string()
                      << "\", but no frame rects could be established."
                      << std::endl;

            return false;
        }

        t_cache.images.push_back(std::move(image));
        return true;
    }

    void AnimationPlayer::createAnimation(
        const std::vector<std::size_t> & t_possibleCacheIndexes,
        const sf::FloatRect & t_bounds,
        const AnimConfig & t_configParam)
    {
        const std::size_t randomCacheIndex{ m_random.from(t_possibleCacheIndexes) };
        const ImageCache & cache{ *m_imageCaches.at(randomCacheIndex) };
        Animation & anim{ getAvailableAnimation() };

        if (t_configParam.is_default)
        {
            anim.config = cache.config;
        }
        else
        {
            anim.config = t_configParam;
        }

        anim.cache_index = cache.index;
        anim.frame_index = 0;
        anim.sec_elapsed = 0.0f;
        anim.is_playing = true;

        setAnimationFrame(anim, 0);

        util::fitAndCenterInside(anim.sprite, t_bounds);
        anim.sprite.setColor(anim.config.color);
    }

    AnimationPlayer::ParsedDirectoryName
        AnimationPlayer::parseDirectoryName(const std::string & t_name) const
    {
        const std::size_t size{ t_name.size() };
        const std::size_t dashIndex{ t_name.rfind('-') };
        const std::size_t xIndex{ t_name.rfind('x') };

        if ((0 == dashIndex) || (dashIndex >= size) || (xIndex >= size) || (xIndex <= dashIndex))
        {
            return {};
        }

        try
        {
            const std::string animName{ t_name.substr(0, dashIndex) };
            const std::string widthStr{ t_name.substr((dashIndex + 1), (dashIndex - xIndex - 1)) };
            const std::string heightStr{ t_name.substr(xIndex + 1) };

            const int width{ std::stoi(widthStr) };
            const int height{ (heightStr.empty()) ? width : std::stoi(heightStr) };

            return { animName, sf::Vector2i(width, height) };
        }
        catch (...)
        {}

        return {};
    }

    AnimationPlayer::Animation & AnimationPlayer::getAvailableAnimation()
    {
        for (Animation & anim : m_animations)
        {
            if (!anim.is_playing)
            {
                return anim;
            }
        }

        if (m_animations.size() > m_maxPlayingAtOnceCount)
        {
            m_animations[0].is_playing = false;
            return m_animations[0];
        }
        else
        {
            return m_animations.emplace_back();
        }
    }

    std::vector<std::size_t>
        AnimationPlayer::findCacheIndexesByName(const std::string & t_name) const
    {
        std::vector<std::size_t> indexes;

        for (std::size_t i(0); i < m_imageCaches.size(); ++i)
        {
            const bool animNameStartsWith{ m_imageCaches.at(i)->animation_name.find(t_name, 0) ==
                                           0 };

            if (animNameStartsWith)
            {
                indexes.push_back(i);
            }
        }

        return indexes;
    }

    void AnimationPlayer::updateAnimation(Animation & t_anim, const float t_elapsedTimeSec) const
    {
        if (!t_anim.is_playing)
        {
            return;
        }

        t_anim.sec_elapsed += t_elapsedTimeSec;

        const ImageCache & cache{ *m_imageCaches.at(t_anim.cache_index) };
        const float frameCount{ static_cast<float>(cache.frame_count) };
        const float durationRatio{ (t_anim.sec_elapsed / t_anim.config.duration_sec) };
        const std::size_t newFrameIndex(static_cast<std::size_t>(frameCount * durationRatio));

        if (newFrameIndex > frameCount)
        {
            t_anim.is_playing = false;
            return;
        }

        if (newFrameIndex == t_anim.frame_index)
        {
            return;
        }

        setAnimationFrame(t_anim, newFrameIndex);
    }

    // TODO this should not iterate but calculate maybe make a function of ImageCache to jump to
    // whatever frame...
    void AnimationPlayer::setAnimationFrame(
        Animation & t_anim, const std::size_t t_newFrameIndex) const
    {
        t_anim.frame_index = t_newFrameIndex;
        const ImageCache & cache{ *m_imageCaches.at(t_anim.cache_index) };

        std::size_t frameCounter{ 0 };
        for (const Image & image : cache.images)
        {
            for (const sf::IntRect & rect : image.rects)
            {
                if (t_newFrameIndex == frameCounter)
                {
                    t_anim.sprite.setTexture(image.texture);
                    t_anim.sprite.setTextureRect(rect);
                    return;
                }

                ++frameCounter;
            }
        }
    }

    std::string AnimationPlayer::ImageCache::toString() const
    {
        const std::string pad("  ");

        std::ostringstream ss;

        ss << "#" << index;
        ss << pad;

        ss << std::setw(14) << std::right << animation_name;
        ss << pad;

        ss << std::setw(3) << std::right << static_cast<int>(frame_size.x);
        ss << "x";
        ss << std::setw(3) << std::left << static_cast<int>(frame_size.y);
        ss << pad;

        ss << "x" << frame_count;
        return ss.str();
    }

} // namespace util
