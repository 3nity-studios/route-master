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

# Patch DeSiGNAR hash.hpp for MSVC alternative token compatibility
if(DEFINED SOURCE_DIR AND EXISTS "${SOURCE_DIR}/include/Designar/hash.hpp")
  file(READ "${SOURCE_DIR}/include/Designar/hash.hpp" HASH_CONTENT)
  string(REPLACE
    "while (i < set_ptr->get_capacity() and"
    "while (i < set_ptr->get_capacity() &&"
    HASH_CONTENT "${HASH_CONTENT}"
  )
  file(WRITE "${SOURCE_DIR}/include/Designar/hash.hpp" "${HASH_CONTENT}")
  message(STATUS "Patched DeSiGNAR hash.hpp for MSVC compatibility")
endif()

# Patch DeSiGNAR array.hpp for MSVC alternative token compatibility
if(DEFINED SOURCE_DIR AND EXISTS "${SOURCE_DIR}/include/Designar/array.hpp")
  file(READ "${SOURCE_DIR}/include/Designar/array.hpp" ARRAY_CONTENT)
  string(REPLACE
    "BaseArray::get_capacity() * RESIZE_FACTOR or"
    "BaseArray::get_capacity() * RESIZE_FACTOR ||"
    ARRAY_CONTENT "${ARRAY_CONTENT}"
  )
  file(WRITE "${SOURCE_DIR}/include/Designar/array.hpp" "${ARRAY_CONTENT}")
  message(STATUS "Patched DeSiGNAR array.hpp for MSVC compatibility")
endif()

# Patch DeSiGNAR queue.hpp for MSVC alternative token compatibility
if(DEFINED SOURCE_DIR AND EXISTS "${SOURCE_DIR}/include/Designar/queue.hpp")
  file(READ "${SOURCE_DIR}/include/Designar/queue.hpp" QUEUE_CONTENT)
  string(REPLACE
    "BaseArray::get_capacity() * RESIZE_FACTOR or"
    "BaseArray::get_capacity() * RESIZE_FACTOR ||"
    QUEUE_CONTENT "${QUEUE_CONTENT}"
  )
  file(WRITE "${SOURCE_DIR}/include/Designar/queue.hpp" "${QUEUE_CONTENT}")
  message(STATUS "Patched DeSiGNAR queue.hpp for MSVC compatibility")
endif()
