cmake_minimum_required(VERSION 2.6)
project(VCACHE)

# version info
set(VCACHE_VERSION 0.0.1)
set(VCACHE_HEXVERSION 0x000001)

# configuration
configure_file(${VCACHE_SOURCE_DIR}/Doxyfile.in ${VCACHE_SOURCE_DIR}/Doxyfile)

# find boost
find_package(Boost 1.44.0 REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${VCACHE_SOURCE_DIR}/include)

# build the actual library
add_library(vcache
    src/vcache/vertex_score.cpp
    src/vcache.cpp
)
target_link_libraries(vcache)

# build the tests
enable_testing()
add_subdirectory(test)
