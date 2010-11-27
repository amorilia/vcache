cmake_minimum_required(VERSION 2.6)
project(VCACHE)

# find boost
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(${VCACHE_SOURCE_DIR}/include)

# build the actual library
add_library(vcache
    src/vcache.cpp
)
target_link_libraries(vcache)

# build the tests
enable_testing()
add_subdirectory(test)
