cmake_minimum_required(VERSION 3.8)
project(tree_ar)

set(CMAKE_CXX_STANDARD 17)

if (APPLE AND EXISTS /usr/local/opt/boost)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/boost")
endif ()

find_package(Boost 1.65.0 REQUIRED COMPONENTS filesystem system)
find_package(PkgConfig)
pkg_search_module(LIBZIP REQUIRED libzip)

set(SOURCE_FILES main.cpp)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIRS})
add_executable(tree_ar ${SOURCE_FILES})
target_link_libraries(tree_ar ${Boost_LIBRARIES} ${LIBZIP_LIBRARIES})
install(TARGETS tree_ar DESTINATION bin)