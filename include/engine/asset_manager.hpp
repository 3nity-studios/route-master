
#pragma once

#include <map>
#include <string>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Engine
{
class AssetManager
{

  private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
    std::map<std::string, sf::SoundBuffer> _sounds;

  public:
    AssetManager()
    {
    }
    ~AssetManager()
    {
    }

    void load_texture(std::string name, std::string file_name);
    sf::Texture &get_texture(std::string name);

    void load_font(std::string name, std::string file_name);
    sf::Font &get_font(std::string name);

    void load_sound(std::string name, std::string file_name);
    sf::SoundBuffer &get_sound(std::string name);

    // expand for sounds and other needs
};
} // namespace Engine
