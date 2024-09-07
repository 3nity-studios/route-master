#include <catch2/catch_test_macros.hpp>

#include "config/include/config.hpp"

TEST_CASE("Name is route-master", "[config]")
{
  config conf;
  REQUIRE(conf.project_name == "route-master");
}
