# Patch DeSiGNAR map.hpp for MSVC template-template parameter compatibility
if(DEFINED SOURCE_DIR AND EXISTS "${SOURCE_DIR}/include/Designar/map.hpp")
  file(READ "${SOURCE_DIR}/include/Designar/map.hpp" CONTENT)
  string(REPLACE
    "using BaseHash = HashSet<Item, CmpWrapperType, LHashTable>;"
    "using BaseHash = HashSet<Item, CmpWrapperType, HashTableType>;"
    CONTENT "${CONTENT}"
  )
  file(WRITE "${SOURCE_DIR}/include/Designar/map.hpp" "${CONTENT}")
  message(STATUS "Patched DeSiGNAR map.hpp for MSVC compatibility")
endif()
