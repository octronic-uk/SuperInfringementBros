project(SuperInfringementBros C)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

SET(CMAKE_MODULE_PATH "${SuperInfringementBros_SOURCE_DIR}/CMakeModules")
message("Using modules path: " ${CMAKE_MODULE_PATH})

aux_source_directory(src SRC_LIST)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(BIN_NAME SuperInfringementBros)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
) 

add_executable(
    ${BIN_NAME}
    ${SRC_LIST}
)

target_link_libraries(
    ${BIN_NAME}
    -lpng
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARY}
    ${SDLMIXER_LIBRARY}
    ${SDL2_TTF_LIBRARY}
)

# Copy Resources
add_custom_command(
    TARGET ${BIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
)
