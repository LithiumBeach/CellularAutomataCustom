cmake_minimum_required(VERSION 2.6)

project(CellularAutomataCustom)

#find SFML's root folder
set(SFML_ROOT ${CMAKE_SOURCE_DIR}/3rdParty/SFML-2.5.1)
#statically link SFML
set(SFML_STATIC_LIBRARIES FALSE)
#set path to FindSFML.cmake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty/SFML-2.5.1/cmake/Modules ${CMAKE_MODULE_PATH})

include_directories(${SFML_INCLUDE_DIR})

file(GLOB src
"*.h"
"*.cpp"
"*.ttf"
)

add_executable(CellularAutomataCustom ${src}
${SFML_DEPENDENCIES} ${SFML_LIBRARIES})

#search the SFML cmake file and include these libraries
find_package(SFML 2 COMPONENTS main audio graphics system window REQUIRED)
#search the TGUI cmake file and include these libraries
#find_package(TGUI REQUIRED graphics window system audio)

target_link_libraries(CellularAutomataCustom ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})


