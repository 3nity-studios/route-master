#include "engine/tmx_core.hpp"

MapLayer::MapLayer(const tmx::Map &map, std::size_t idx)
{
    const auto &layers = map.getLayers();
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cout << "Map is not orthogonal - nothing will be drawn" << std::endl;
    }
    else if (idx >= layers.size())
    {
        std::cout << "Layer index " << idx << " is out of range, layer count is " << layers.size() << std::endl;
    }
    else if (layers[idx]->getType() != tmx::Layer::Type::Tile)
    {
        std::cout << "layer " << idx << " is not a Tile layer..." << std::endl;
    }

    else
    {
        // round the chunk size to the nearest tile
        const auto tile_size = map.getTileSize();
        _chunk_size.x = std::floor(_chunk_size.x / static_cast<float>(tile_size.x)) * static_cast<float>(tile_size.x);
        _chunk_size.y = std::floor(_chunk_size.y / static_cast<float>(tile_size.y)) * static_cast<float>(tile_size.y);
        _map_tile_size.x = map.getTileSize().x;
        _map_tile_size.y = map.getTileSize().y;
        const auto &layer = layers[idx]->getLayerAs<tmx::TileLayer>();
        create_chunks(map, layer);

        auto map_size = map.getBounds();
        _global_bounds.size = {map_size.width, map_size.height};
    }
}

const sf::FloatRect &MapLayer::get_global_bounds() const
{
    return _global_bounds;
}

void MapLayer::set_tile(std::int32_t tile_x, std::int32_t tile_y, tmx::TileLayer::Tile tile, bool refresh = true)
{
    sf::Vector2u chunk_locale;
    const auto &selected_chunk = get_chunk_and_transform(tile_x, tile_y, chunk_locale);
    selected_chunk->set_tile(static_cast<std::int32_t>(chunk_locale.x), static_cast<std::int32_t>(chunk_locale.y), tile,
                           refresh);
}

tmx::TileLayer::Tile MapLayer::get_tile(std::int32_t tile_x, std::int32_t tile_y)
{
    sf::Vector2u chunk_locale;
    const auto &selected_chunk = get_chunk_and_transform(tile_x, tile_y, chunk_locale);
    return selected_chunk->get_tile(static_cast<std::int32_t>(chunk_locale.x), static_cast<std::int32_t>(chunk_locale.y));
}

void MapLayer::set_color(std::int32_t tile_x, std::int32_t tile_y, sf::Color color, bool refresh = true)
{
    sf::Vector2u chunk_locale;
    const auto &selected_chunk = get_chunk_and_transform(tile_x, tile_y, chunk_locale);
    selected_chunk->set_color(static_cast<std::int32_t>(chunk_locale.x), static_cast<std::int32_t>(chunk_locale.y), color,
                            refresh);
}

sf::Color MapLayer::get_color(std::int32_t tile_x, std::int32_t tile_y)
{
    sf::Vector2u chunk_locale;
    const auto &selected_chunk = get_chunk_and_transform(tile_x, tile_y, chunk_locale);
    return selected_chunk->get_color(static_cast<std::int32_t>(chunk_locale.x), static_cast<std::int32_t>(chunk_locale.y));
}

void MapLayer::set_offset(sf::Vector2f offset)
{
    _offset = offset;
}

sf::Vector2f MapLayer::get_offset() const
{
    return _offset;
}

void MapLayer::update(sf::Time elapsed)
{
    for (auto &c : _visible_chunks)
    {
        for (AnimationState &as : c->get_active_animations())
        {
            as.current_time += elapsed;

            tmx::TileLayer::Tile tile;
            std::int32_t anim_time = 0;
            auto x = as.anim_tile.animation.frames.begin();
            while (anim_time < as.current_time.asMilliseconds())
            {
                if (x == as.anim_tile.animation.frames.end())
                {
                    x = as.anim_tile.animation.frames.begin();
                    as.current_time -= sf::milliseconds(anim_time);
                    anim_time = 0;
                }

                tile.ID = x->tileID;
                anim_time += x->duration;
                x++;
            }

            set_tile(static_cast<std::int32_t>(as.tile_cords.x), static_cast<std::int32_t>(as.tile_cords.y), tile);
        }
    }
}

std::unique_ptr<MapLayer::Chunk> &MapLayer::get_chunk_and_transform(std::int32_t x, std::int32_t y, sf::Vector2u &chunk_relative)
{
    std::uint32_t chunk_x = (static_cast<uint>(x) * _map_tile_size.x) / static_cast<std::uint32_t>(_chunk_size.x);
    std::uint32_t chunk_y = (static_cast<uint>(y) * _map_tile_size.y) / static_cast<std::uint32_t>(_chunk_size.y);
    chunk_relative.x = ((static_cast<uint>(x) * _map_tile_size.x) - chunk_x * static_cast<std::uint32_t>(_chunk_size.x)) /
                      _map_tile_size.x;
    chunk_relative.y = ((static_cast<uint>(y) * _map_tile_size.y) - chunk_y * static_cast<std::uint32_t>(_chunk_size.y)) /
                      _map_tile_size.y;
    return _chunks[chunk_x + chunk_y * _chunk_count.x];
}

void MapLayer::create_chunks(const tmx::Map &map, const tmx::TileLayer &layer)
{
    // look up all the tile sets and load the textures
    const auto &tile_sets = map.getTilesets();
    const auto &layer_ids = layer.getTiles();
    std::uint32_t max_id = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset *> used_tile_sets;

    for (auto i = tile_sets.rbegin(); i != tile_sets.rend(); ++i)
    {
        for (const auto &tile : layer_ids)
        {
            if (tile.ID >= i->getFirstGID() && tile.ID < max_id)
            {
                used_tile_sets.push_back(&(*i));
                break;
            }
        }
        max_id = i->getFirstGID();
    }

    sf::Image fallback{{2, 2}, sf::Color::Magenta};
    for (const auto &ts : used_tile_sets)
    {
        const auto &path = ts->getImagePath();

        std::unique_ptr<sf::Texture> new_texture = std::make_unique<sf::Texture>();
        sf::Image img;
        if (!img.loadFromFile(path))
        {
            static_cast<void>(new_texture->loadFromImage(fallback));
        }
        else
        {
            if (ts->hasTransparency())
            {
                auto transparency = ts->getTransparencyColour();
                img.createMaskFromColor({transparency.r, transparency.g, transparency.b, transparency.a});
            }
            static_cast<void>(new_texture->loadFromImage(img));
        }
        _texture_resource.insert(std::make_pair(path, std::move(new_texture)));
    }

    // calculate the number of chunks in the layer
    // and create each one
    const auto bounds = map.getBounds();
    _chunk_count.x = static_cast<std::uint32_t>(std::ceil(bounds.width / _chunk_size.x));
    _chunk_count.y = static_cast<std::uint32_t>(std::ceil(bounds.height / _chunk_size.y));

    sf::Vector2u tile_size(map.getTileSize().x, map.getTileSize().y);

    for (auto y = 0u; y < _chunk_count.y; ++y)
    {
        sf::Vector2f tile_count(_chunk_size.x / static_cast<float>(tile_size.x),
                               _chunk_size.y / static_cast<float>(tile_size.y));
        for (auto x = 0u; x < _chunk_count.x; ++x)
        {
            // calculate size of each Chunk (clip against map)
            if ((static_cast<float>(x + 1)) * _chunk_size.x > bounds.width)
            {
                tile_count.x =
                    (bounds.width - static_cast<float>(x) * _chunk_size.x) / static_cast<float>(map.getTileSize().x);
            }
            if ((static_cast<float>(y + 1)) * _chunk_size.y > bounds.height)
            {
                tile_count.y =
                    (bounds.height - static_cast<float>(y) * _chunk_size.y) / static_cast<float>(map.getTileSize().y);
            }
            // m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
            //    sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, map.getTileCount().x,
            //    m_textureResource));
            _chunks.emplace_back(std::make_unique<Chunk>(
                layer, used_tile_sets,
                sf::Vector2f(static_cast<float>(x) * _chunk_size.x, static_cast<float>(y) * _chunk_size.y), tile_count,
                tile_size, map.getTileCount().x, _texture_resource, map.getAnimatedTiles()));
        }
    }
}

void MapLayer::update_visibility(const sf::View &view) const
{
    sf::Vector2f view_corner = view.getCenter();
    view_corner -= view.getSize() / 2.f;

    std::int32_t pos_x = static_cast<std::int32_t>(std::floor(view_corner.x / _chunk_size.x));
    std::int32_t pos_y = static_cast<std::int32_t>(std::floor(view_corner.y / _chunk_size.y));
    std::int32_t pos_x2 = static_cast<std::int32_t>(std::ceil((view_corner.x + view.getSize().x) / _chunk_size.x));
    std::int32_t pos_y2 = static_cast<std::int32_t>(std::ceil((view_corner.y + view.getSize().x) / _chunk_size.y));

    std::vector<Chunk *> visible;
    for (auto y = pos_y; y < pos_y2; ++y)
    {
        for (auto x = pos_x; x < pos_x2; ++x)
        {
            std::size_t idx = static_cast<std::size_t>(y * std::int32_t(_chunk_count.x) + x);
            if (idx >= 0u && idx < _chunks.size() && !_chunks[idx]->empty())
            {
                visible.push_back(_chunks[idx].get());
            }
        }
    }

    std::swap(_visible_chunks, visible);
}

void MapLayer::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    states.transform.translate(_offset);

    // calc view coverage and draw nearest chunks
    update_visibility(rt.getView());
    for (const auto &c : _visible_chunks)
    {
        rt.draw(*c, states);
    }
}

MapLayer::Chunk::Chunk(const tmx::TileLayer &layer, std::vector<const tmx::Tileset *> tilesets, const sf::Vector2f &position,
                const sf::Vector2f &tile_count, const sf::Vector2u &tile_size, std::size_t row_size, TextureResource &tr,
                const std::map<std::uint32_t, tmx::Tileset::Tile> &anim_tiles)
    : _anim_tiles(anim_tiles)
{
    setPosition(position);
    _layer_opacity = static_cast<std::uint8_t>(layer.getOpacity() / 1.f * 255.f);

    sf::Color vert_colour = sf::Color(200, 200, 200, _layer_opacity);
    auto offset = layer.getOffset();
    _layer_offset = {static_cast<float>(offset.x), static_cast<float>(offset.y)};
    _chunk_tile_count = {tile_count.x, tile_count.y};
    _map_tile_size = tile_size;

    const auto &tile_ids = layer.getTiles();

    // go through the tiles and create all arrays (for latter manipulation)
    for (const auto &ts : tilesets)
    {
        if (ts->getImagePath().empty())
        {
            tmx::Logger::log("This example does not support Collection of Images tilesets", tmx::Logger::Type::Info);
            tmx::Logger::log("Chunks using " + ts->getName() + " will not be created", tmx::Logger::Type::Info);
            continue;
        }
        _chunk_arrays.emplace_back(std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second, *ts));
    }
    std::size_t x_pos = static_cast<std::size_t>(position.x / static_cast<float>(tile_size.x));
    std::size_t y_pos = static_cast<std::size_t>(position.y / static_cast<float>(tile_size.y));
    for (auto y = y_pos; static_cast<float>(y) < static_cast<float>(y_pos) + tile_count.y; ++y)
    {
        for (auto x = x_pos; static_cast<float>(x) < static_cast<float>(x_pos) + tile_count.x; ++x)
        {
            auto idx = (y * row_size + x);
            _chunk_tile_ids.emplace_back(tile_ids[idx]);
            _chunk_colors.emplace_back(vert_colour);
        }
    }
    generate_tiles(true);
}

void MapLayer::Chunk::generate_tiles(bool register_animation = false)
{
    if (register_animation)
    {
        _active_animations.clear();
    }
    for (const auto &ca : _chunk_arrays)
    {
        std::uint32_t idx = 0;
        std::uint32_t x_pos = static_cast<std::uint32_t>(getPosition().x / static_cast<float>(_map_tile_size.x));
        std::uint32_t y_pos = static_cast<std::uint32_t>(getPosition().y / static_cast<float>(_map_tile_size.y));
        for (auto y = y_pos; static_cast<float>(y) < static_cast<float>(y_pos) + _chunk_tile_count.y; ++y)
        {
            for (auto x = x_pos; static_cast<float>(x) < static_cast<float>(x_pos) + _chunk_tile_count.x; ++x)
            {
                if (idx < _chunk_tile_ids.size() && _chunk_tile_ids[idx].ID >= ca->first_gid &&
                    _chunk_tile_ids[idx].ID <= ca->last_gid)
                {
                    if (register_animation && _anim_tiles.find(_chunk_tile_ids[idx].ID) != _anim_tiles.end())
                    {
                        AnimationState as;
                        as.anim_tile = _anim_tiles[_chunk_tile_ids[idx].ID];
                        as.start_time = sf::milliseconds(0);
                        as.tile_cords = sf::Vector2u(x, y);
                        _active_animations.push_back(as);
                    }

                    sf::Vector2f tile_offset(static_cast<float>(x * _map_tile_size.x),
                                            static_cast<float>(y * _map_tile_size.y) + static_cast<float>(_map_tile_size.y) -
                                                static_cast<float>(ca->tile_set_size.y));

                    auto id_index = _chunk_tile_ids[idx].ID - ca->first_gid;
                    sf::Vector2f tile_index(sf::Vector2i(static_cast<int>(id_index % ca->ts_tile_count.x),
                                                        static_cast<int>(id_index / ca->ts_tile_count.x)));
                    tile_index.x *= static_cast<float>(ca->tile_set_size.x);
                    tile_index.y *= static_cast<float>(ca->tile_set_size.y);
                    Tile tile = {
                        sf::Vertex{tile_offset - getPosition(), _chunk_colors[idx], tile_index},
                        sf::Vertex{
                            tile_offset - getPosition() + sf::Vector2f(static_cast<float>(ca->tile_set_size.x), 0.f),
                            _chunk_colors[idx], tile_index + sf::Vector2f(static_cast<float>(ca->tile_set_size.x), 0.f)},
                        sf::Vertex{tile_offset - getPosition() +
                                       sf::Vector2f(sf::Vector2u(ca->tile_set_size.x, ca->tile_set_size.y)),
                                   _chunk_colors[idx],
                                   tile_index + sf::Vector2f(sf::Vector2u(ca->tile_set_size.x, ca->tile_set_size.y))},
                        sf::Vertex{tile_offset - getPosition(), _chunk_colors[idx], tile_index},
                        sf::Vertex{tile_offset - getPosition() +
                                       sf::Vector2f(sf::Vector2u(ca->tile_set_size.x, ca->tile_set_size.y)),
                                   _chunk_colors[idx],
                                   tile_index + sf::Vector2f(sf::Vector2u(ca->tile_set_size.x, ca->tile_set_size.y))},
                        sf::Vertex{
                            tile_offset - getPosition() + sf::Vector2f(0.f, static_cast<float>(ca->tile_set_size.y)),
                            _chunk_colors[idx], tile_index + sf::Vector2f(0.f, static_cast<float>(ca->tile_set_size.y))}};
                    do_flips(_chunk_tile_ids[idx].flipFlags, &tile[0].texCoords, &tile[1].texCoords, &tile[2].texCoords,
                            &tile[3].texCoords, &tile[4].texCoords, &tile[5].texCoords);
                    ca->add_tile(tile);
                }
                idx++;
            }
        }
    }
}

std::vector<MapLayer::AnimationState> &MapLayer::Chunk::get_active_animations()
{
    return _active_animations;
}

tmx::TileLayer::Tile MapLayer::Chunk::get_tile(std::int32_t x, std::int32_t y) const
{
    return _chunk_tile_ids[static_cast<uint>(calc_index_from(x, y))];
}

void MapLayer::Chunk::set_tile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh)
{
    _chunk_tile_ids[static_cast<uint>(calc_index_from(x, y))] = tile;
    maybe_regenerate(refresh);
}

sf::Color MapLayer::Chunk::get_color(std::int32_t x, std::int32_t y) const
{
    return _chunk_colors[static_cast<uint>(calc_index_from(x, y))];
}

void MapLayer::Chunk::set_color(std::int32_t x, std::int32_t y, sf::Color color, bool refresh)
{
    _chunk_colors[static_cast<uint>(calc_index_from(x, y))] = color;
    maybe_regenerate(refresh);
}

void MapLayer::Chunk::maybe_regenerate(bool refresh)
{
    if (refresh)
    {
        for (const auto &ca : _chunk_arrays)
        {
            ca->reset();
        }
        generate_tiles();
    }
}

std::int32_t MapLayer::Chunk::calc_index_from(std::int32_t x, std::int32_t y) const
{
    return x + y * static_cast<std::int32_t>(_chunk_tile_count.x);
}

bool MapLayer::Chunk::empty() const
{
    return _chunk_arrays.empty();
}

void MapLayer::Chunk::flip_y(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
                            sf::Vector2f *v5)
{
    // Flip Y
    sf::Vector2f tmp0 = *v0;
    v0->y = v5->y;
    v3->y = v5->y;
    v5->y = tmp0.y;
    sf::Vector2f tmp2 = *v2;
    v2->y = v1->y;
    v4->y = v1->y;
    v1->y = tmp2.y;
}

void MapLayer::Chunk::flip_x(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
                            sf::Vector2f *v5)
{
    // Flip X
    sf::Vector2f tmp0 = *v0;
    v0->x = v1->x;
    v3->x = v1->x;
    v1->x = tmp0.x;
    sf::Vector2f tmp2 = *v2;
    v2->x = v5->x;
    v4->x = v5->x;
    v5->x = tmp2.x;
}

void MapLayer::Chunk::flip_d(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3, sf::Vector2f *v4,
                            sf::Vector2f *v5)
{
    // Diagonal flip
    sf::Vector2f tmp2 = *v2;
    *v2 = *v4;
    *v4 = tmp2;
    sf::Vector2f tmp0 = *v0;
    *v0 = *v3;
    *v3 = tmp0;
    sf::Vector2f tmp1 = *v1;
    *v1 = *v5;
    *v5 = tmp1;
}

void MapLayer::Chunk::do_flips(std::uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                              sf::Vector2f *v4, sf::Vector2f *v5)
{
    // 0000 = no change
    // 0100 = vertical = swap y axis
    // 1000 = horizontal = swap x axis
    // 1100 = horiz + vert = swap both axes = horiz+vert = rotate 180 degrees
    // 0010 = diag = rotate 90 degrees right and swap x axis
    // 0110 = diag+vert = rotate 270 degrees right
    // 1010 = horiz+diag = rotate 90 degrees right
    // 1110 = horiz+vert+diag = rotate 90 degrees right and swap y axis
    if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) && !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // Shortcircuit tests for nothing to do
        return;
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) && (bits & tmx::TileLayer::FlipFlag::Vertical) &&
             !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0100
        flip_y(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) && !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
             !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1000
        flip_x(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) && (bits & tmx::TileLayer::FlipFlag::Vertical) &&
             !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1100
        flip_y(v0, v1, v2, v3, v4, v5);
        flip_x(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) && !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
             (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0010
        flip_d(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) && (bits & tmx::TileLayer::FlipFlag::Vertical) &&
             (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0110
        flip_x(v0, v1, v2, v3, v4, v5);
        flip_d(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) && !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
             (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1010
        flip_y(v0, v1, v2, v3, v4, v5);
        flip_d(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) && (bits & tmx::TileLayer::FlipFlag::Vertical) &&
             (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1110
        flip_y(v0, v1, v2, v3, v4, v5);
        flip_x(v0, v1, v2, v3, v4, v5);
        flip_d(v0, v1, v2, v3, v4, v5);
    }
}

void MapLayer::Chunk::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto &a : _chunk_arrays)
    {
        rt.draw(*a, states);
    }
}

MapLayer::Chunk::ChunkArray::ChunkArray(const sf::Texture &t, const tmx::Tileset &ts) : _texture(t)
{
    auto tex_size = get_texture_size();
    tile_set_size = ts.getTileSize();
    ts_tile_count.x = tex_size.x / tile_set_size.x;
    ts_tile_count.y = tex_size.y / tile_set_size.y;
    first_gid = ts.getFirstGID();
    last_gid = ts.getLastGID();
}

void MapLayer::Chunk::ChunkArray::reset()
{
    _vertices.clear();
}

void MapLayer::Chunk::ChunkArray::add_tile(const Chunk::Tile &tile)
{
    for (const auto &v : tile)
    {
        _vertices.push_back(v);
    }
}

sf::Vector2u MapLayer::Chunk::ChunkArray::get_texture_size() const
{
    return _texture.getSize();
}

void MapLayer::Chunk::ChunkArray::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    states.texture = &_texture;
    rt.draw(_vertices.data(), _vertices.size(), sf::PrimitiveType::Triangles, states);
}
