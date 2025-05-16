cmake_minimum_required(VERSION 3.5...4.0)
project(random-tiling-generator VERSION 1.0 LANGUAGES C)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE src/random-tiling-generator.c)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/init-win.c)
elseif(UNIX)
    target_sources(${PROJECT_NAME} PRIVATE src/init-pos.c)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "inc")