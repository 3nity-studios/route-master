#include "asset_manager.hpp"

namespace Engine
{
void AssetManager::load_texture(std::string name, std::string file_name)
{
    sf::Texture tex;

    if (tex.loadFromFile(file_name))
    {
        this->_textures[name] = tex;
    }
}

sf::Texture &AssetManager::get_texture(std::string name)
{
    return this->_textures.at(name);
}

void AssetManager::load_font(std::string name, std::string file_name)
{
    sf::Font font;

    if (font.loadFromFile(file_name))
    {
        this->_fonts[name] = font;
    }
}

sf::Font &AssetManager::get_font(std::string name)
{
    return this->_fonts.at(name);
}

void AssetManager::load_sound(std::string name, std::string file_name)
{
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(file_name))
    {
        this->_sounds[name] = buffer;
    }
}

sf::SoundBuffer &AssetManager::get_sound(std::string name)
{
    return this->_sounds.at(name);
}
} // namespace Engine
