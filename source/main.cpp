#include "config/game.hpp"
#include "config/global.hpp"

#include <Designar/array.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <tmxlite/Tileset.hpp>

int main()
{
    tmx::Tileset tileset{"."};
    Game();
    return 0;
}
