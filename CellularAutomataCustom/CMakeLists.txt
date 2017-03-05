cmake_minimum_required(VERSION 2.6)

project(Walk_The_Dinosaur)

#find SFML's root folder
set(SFML_ROOT ${CMAKE_SOURCE_DIR}/3rdParty/SFML-2.3.2)
#statically link SFML
set(SFML_STATIC_LIBRARIES TRUE)
#set path to FindSFML.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/3rdParty/SFML-2.3.2/cmake/Modules" ${CMAKE_MODULE_PATH})

include_directories(${SFML_INCLUDE_DIR})

file(GLOB src
"*.h"
"*.cpp"
)

add_executable(Walk_The_Dinosaur ${src}
${SFML_DEPENDENCIES} ${SFML_LIBRARIES})

#search the SFML cmake file and include these libraries
find_package(SFML 2 REQUIRED graphics window system audio)

target_link_libraries(Walk_The_Dinosaur ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})