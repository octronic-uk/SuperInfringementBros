project(SuperInfringementBros C)
cmake_minimum_required(VERSION 3.0)

aux_source_directory(src SRC_LIST)

find_package(SDL2 REQUIRED)

set(BIN_NAME SuperInfringementBros)

include_directories(
    ${SDL2_INCLUDE_DIR}
) 

add_executable(
    ${BIN_NAME}
    ${SRC_LIST}
)

target_link_libraries(
    ${BIN_NAME}
    ${SDL2_LIBRARIES}
)


