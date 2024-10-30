/*
Creates an SFML drawable from an Orthogonal tmx map layer.
This is an example of drawing with SFML - not all features
are implemented.
*/

#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

class MapLayer final : public sf::Drawable
{
  public:
    MapLayer(const tmx::Map &map, std::size_t idx);
    ~MapLayer() = default;
    MapLayer(const MapLayer &) = delete;
    MapLayer &operator=(const MapLayer &) = delete;

    const sf::FloatRect &get_global_bounds() const;

    void set_tile(std::int32_t tile_x, std::int32_t tile_y, tmx::TileLayer::Tile tile, bool refresh);
    tmx::TileLayer::Tile get_tile(std::int32_t tile_x, std::int32_t tile_y);

    void set_color(std::int32_t tile_x, std::int32_t tile_y, sf::Color color, bool refresh);
    sf::Color get_color(std::int32_t tile_x, std::int32_t tile_y);

    void set_offset(sf::Vector2f offset);
    sf::Vector2f get_offset() const;

    void update(sf::Time elapsed);

  private:
    // increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
    sf::Vector2f _chunk_size = sf::Vector2f(1024.f, 1024.f);
    // sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
    sf::Vector2u _chunk_count;
    sf::Vector2u _map_tile_size; // general Tilesize of Map
    sf::FloatRect _global_bounds;
    sf::Vector2f _offset;

    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
    TextureResource _texture_resource;

    struct AnimationState
    {
        sf::Vector2u tile_cords;
        sf::Time start_time;
        sf::Time current_time;
        tmx::Tileset::Tile anim_tile;
        std::uint8_t flip_flags;
    };

    class Chunk;

    std::vector<std::unique_ptr<Chunk>> _chunks;
    mutable std::vector<Chunk *> _visible_chunks;

    std::unique_ptr<Chunk> &get_chunk_and_transform(std::int32_t x, std::int32_t y, sf::Vector2u &chunk_relative);
    void create_chunks(const tmx::Map &map, const tmx::TileLayer &layer);
    void update_visibility(const sf::View &view) const;
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};

class MapLayer::Chunk final : public sf::Transformable, public sf::Drawable
{
  public:
    using Tile = std::array<sf::Vertex, 6u>;

    Chunk(const tmx::TileLayer &layer, std::vector<const tmx::Tileset *> tilesets, const sf::Vector2f &position,
          const sf::Vector2f &tile_count, const sf::Vector2u &tile_size, std::size_t row_size, TextureResource &tr,
          const std::map<std::uint32_t, tmx::Tileset::Tile> &anim_tiles);

    ~Chunk() = default;
    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;

    void generate_tiles(bool register_animation);

    std::vector<AnimationState> &get_active_animations();

    tmx::TileLayer::Tile get_tile(std::int32_t x, std::int32_t y) const;
    void set_tile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh);

    sf::Color get_color(std::int32_t x, std::int32_t y) const;
    void set_color(std::int32_t x, std::int32_t y, sf::Color color, bool refresh);

    void maybe_regenerate(bool refresh);

    std::int32_t calc_index_from(std::int32_t x, std::int32_t y) const;

    bool empty() const;

    void flip_y(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void flip_x(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void flip_d(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void do_flips(std::uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                 sf::Vector2f *v4, sf::Vector2f *v5);

  private:
    class ChunkArray;

    std::uint8_t _layer_opacity;
    sf::Vector2f _layer_offset;
    sf::Vector2u _map_tile_size;
    sf::Vector2f _chunk_tile_count;
    std::vector<tmx::TileLayer::Tile> _chunk_tile_ids;        // stores all tiles in this chunk for later manipulation
    std::vector<sf::Color> _chunk_colors;                     // stores colors for extended color effects
    std::map<std::uint32_t, tmx::Tileset::Tile> _anim_tiles;  // animation catalogue
    std::vector<AnimationState> _active_animations;           // animations to be done in this chunk
    std::vector<std::unique_ptr<ChunkArray>> _chunk_arrays;

    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};

class MapLayer::Chunk::ChunkArray final : public sf::Drawable
{
  public:

    tmx::Vector2u tile_set_size;
    sf::Vector2u ts_tile_count;
    std::uint32_t first_gid, last_gid;

    explicit ChunkArray(const sf::Texture &t, const tmx::Tileset &ts);

    ~ChunkArray() = default;
    ChunkArray(const ChunkArray &) = delete;
    ChunkArray &operator=(const ChunkArray &) = delete;

    void reset();

    void add_tile(const Chunk::Tile &tile);

    sf::Vector2u get_texture_size() const;

  private:
    const sf::Texture &_texture;
    std::vector<sf::Vertex> _vertices;
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};
