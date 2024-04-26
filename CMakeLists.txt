cmake_minimum_required(VERSION 3.27)
project(BugLife)

set(CMAKE_CXX_STANDARD 17)

add_executable(BugLife main.cpp Bug.cpp Crawler.cpp Hopper.cpp Bug.cpp Crawler.cpp Crawler.h Hopper.h Hopper.cpp Teleporter.h Teleporter.cpp
        SuperBug.cpp
        SuperBug.h)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics REQUIRED)
include_directories(C:/SFML/include)
target_link_libraries(BugLife sfml-graphics sfml-window sfml-system)