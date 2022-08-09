cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME TrainDodge)
set(PLAYDATE_GAME_DEVICE TrainDodge)

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/main.cpp

        ${ARDUBOY_SOURCE}
        ${ARDUBOYPLAYTUNE_SOURCE}
        )

if (TOOLCHAIN STREQUAL "armgcc")
    add_definitions(-D_PLAYDATE_DEVICE)
    set(PLAYDATE_TARGET_NAME ${PLAYDATE_GAME_NAME})
    add_executable(${PLAYDATE_TARGET_NAME} ${SETUP_SOURCE} ${SOURCE})
else()
    set(PLAYDATE_TARGET_NAME ${PLAYDATE_GAME_NAME})
    add_library(${PLAYDATE_TARGET_NAME} SHARED ${SOURCE})
endif()

set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")
include(${CMAKE_CURRENT_SOURCE_DIR}/../playdate_game.patched.cmake)


