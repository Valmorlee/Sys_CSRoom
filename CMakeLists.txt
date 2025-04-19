
# Valmorlee Written On 4.19

CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

project(Sys_CSRoom
        LANGUAGES CXX
        VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# =============== FTXUI ==================

set(FTXUI_ROOT ${PROJECT_SOURCE_DIR}/FTXUI)

include_directories(${FTXUI_ROOT}/include)
aux_source_directory(${FTXUI_ROOT}/src/ftxui FTXUI_SRC)

# =============== Basic ==================

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

add_executable(Sys_CSRoom
        sys_main.cpp
        ${FTXUI_SRC}
        ${SRC}
)

target_link_libraries(Sys_CSRoom PRIVATE
        ${FTXUI_ROOT}/lib/libftxui-component.a
        ${FTXUI_ROOT}/lib/libftxui-dom.a
        ${FTXUI_ROOT}/lib/libftxui-screen.a
)