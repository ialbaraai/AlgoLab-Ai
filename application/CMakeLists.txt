cmake_minimum_required(VERSION 3.20)
project(AlgoLabAI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(LIBS_DIR ${PROJECT_ROOT}/libs)

set(IMGUI_DIR ${LIBS_DIR}/ImGui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

set(TINYFD_DIR ${LIBS_DIR}/tinyfiledialogs)
set(TINYFD_SRC ${TINYFD_DIR}/tinyfiledialogs.c)

# SDL (CLEAN WAY)
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

set(SOURCES main.cpp)

file(GLOB_RECURSE ALG_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/application/src/algorithms/*.cpp)
file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/application/src/applications/*.cpp)
file(GLOB_RECURSE UTIL_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/application/src/utilities/*.cpp)

list(APPEND SOURCES ${ALG_SRC} ${APP_SRC} ${UTIL_SRC} ${IMGUI_SRC} ${TINYFD_SRC})

add_executable(AlgoLabAI ${SOURCES})

target_include_directories(AlgoLabAI PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${PROJECT_ROOT}/assets
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${TINYFD_DIR}
)

target_link_libraries(AlgoLabAI PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

add_custom_command(TARGET AlgoLabAI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ROOT}/assets
    $<TARGET_FILE_DIR:AlgoLabAI>/assets
)