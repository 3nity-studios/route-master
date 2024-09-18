#include "config/game.hpp"
#include "config/global.hpp"

#include <Designar/array.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <tmxlite/Tileset.hpp>

int main()
{
    Designar::nat_t numero = 82;
    std::cout << numero;
    tmx::Tileset tileset{"."};
    Game();
    return 0;
}
