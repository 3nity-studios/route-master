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

    const sf::FloatRect &getGlobalBounds() const;

    void setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile, bool refresh);
    tmx::TileLayer::Tile getTile(std::int32_t tileX, std::int32_t tileY);

    void setColor(std::int32_t tileX, std::int32_t tileY, sf::Color color, bool refresh);
    sf::Color getColor(std::int32_t tileX, std::int32_t tileY);

    void setOffset(sf::Vector2f offset);
    sf::Vector2f getOffset() const;

    void update(sf::Time elapsed);

  private:
    // increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
    sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
    // sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
    sf::Vector2u m_chunkCount;
    sf::Vector2u m_mapTileSize; // general Tilesize of Map
    sf::FloatRect m_globalBounds;
    sf::Vector2f m_offset;

    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
    TextureResource m_textureResource;

    struct AnimationState
    {
        sf::Vector2u tileCords;
        sf::Time startTime;
        sf::Time currentTime;
        tmx::Tileset::Tile animTile;
        std::uint8_t flipFlags;
    };

    class Chunk;

    std::vector<std::unique_ptr<Chunk>> m_chunks;
    mutable std::vector<Chunk *> m_visibleChunks;

    std::unique_ptr<Chunk> &getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u &chunkRelative);
    void createChunks(const tmx::Map &map, const tmx::TileLayer &layer);
    void updateVisibility(const sf::View &view) const;
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};

class MapLayer::Chunk final : public sf::Transformable, public sf::Drawable
{
  public:
    using Tile = std::array<sf::Vertex, 6u>;

    Chunk(const tmx::TileLayer &layer, std::vector<const tmx::Tileset *> tilesets, const sf::Vector2f &position,
          const sf::Vector2f &tileCount, const sf::Vector2u &tileSize, std::size_t rowSize, TextureResource &tr,
          const std::map<std::uint32_t, tmx::Tileset::Tile> &animTiles);

    ~Chunk() = default;
    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;

    void generateTiles(bool registerAnimation);

    std::vector<AnimationState> &getActiveAnimations();

    tmx::TileLayer::Tile getTile(std::int32_t x, std::int32_t y) const;
    void setTile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh);

    sf::Color getColor(std::int32_t x, std::int32_t y) const;
    void setColor(std::int32_t x, std::int32_t y, sf::Color color, bool refresh);

    void maybeRegenerate(bool refresh);

    std::int32_t calcIndexFrom(std::int32_t x, std::int32_t y) const;

    bool empty() const;

    void flipY(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void flipX(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void flipD(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
               sf::Vector2f *v5);

    void doFlips(std::uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                 sf::Vector2f *v4, sf::Vector2f *v5);

  private:
    class ChunkArray;

    std::uint8_t layerOpacity;
    sf::Vector2f layerOffset;
    sf::Vector2u mapTileSize;
    sf::Vector2f chunkTileCount;
    std::vector<tmx::TileLayer::Tile> m_chunkTileIDs;        // stores all tiles in this chunk for later manipulation
    std::vector<sf::Color> m_chunkColors;                    // stores colors for extended color effects
    std::map<std::uint32_t, tmx::Tileset::Tile> m_animTiles; // animation catalogue
    std::vector<AnimationState> m_activeAnimations;          // animations to be done in this chunk
    std::vector<std::unique_ptr<ChunkArray>> m_chunkArrays;

    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};

class MapLayer::Chunk::ChunkArray final : public sf::Drawable
{
  public:

    tmx::Vector2u tileSetSize;
    sf::Vector2u tsTileCount;
    std::uint32_t m_firstGID, m_lastGID;

    explicit ChunkArray(const sf::Texture &t, const tmx::Tileset &ts);

    ~ChunkArray() = default;
    ChunkArray(const ChunkArray &) = delete;
    ChunkArray &operator=(const ChunkArray &) = delete;

    void reset();

    void addTile(const Chunk::Tile &tile);

    sf::Vector2u getTextureSize() const;

  private:
    const sf::Texture &m_texture;
    std::vector<sf::Vertex> m_vertices;
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;
};
