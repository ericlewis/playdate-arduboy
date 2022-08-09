cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 11)
add_definitions(-D_PLAYDATE)

if (APPLE)
    execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(LIB_EXTENSION dylib)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
    set(SDK $ENV{PLAYDATE_SDK_PATH})
    set(LIB_EXTENSION dll)
endif()

project(globalProject)

set(SETUP_SOURCE
        ${SDK}/C_API/buildsupport/setup.c
        )

set(ARDUBOY_INCLUDES
        ../Arduboy2Playdate
        ../Arduboy2Playdate/ArduboyTones
        ../Arduboy2Playdate/ArduboyPlaytune
        )

set(ARDUBOY_SOURCE
        ../Arduboy2Playdate/Arduboy2.cpp
        ../Arduboy2Playdate/Arduboy2Audio.cpp
        ../Arduboy2Playdate/Arduboy2Beep.cpp
        ../Arduboy2Playdate/Arduboy2Common.cpp
        ../Arduboy2Playdate/Arduboy2Core.cpp
        ../Arduboy2Playdate/Arduboy2Data.cpp
        ../Arduboy2Playdate/Sprites.cpp
        ../Arduboy2Playdate/SpritesB.cpp
        )

set(ARDUBOYPLAYTUNE_SOURCE
        ../Arduboy2Playdate/ArduboyPlaytune/ArduboyPlaytune.cpp
        )

set(ARDUBOYTONES_SOURCE
        ../Arduboy2Playdate/ArduboyTones/ArduboyTones.cpp
        )

add_subdirectory("Arduboy3d")
add_subdirectory("Arduminer")
add_subdirectory("Begemmed")
add_subdirectory("Bone Shakers")
add_subdirectory("Bubble Pop")
add_subdirectory("CastleBoy")
add_subdirectory("CircuitDude")
add_subdirectory("Crates")
add_subdirectory("Dice of Fate")
add_subdirectory("Escaper Droid")
add_subdirectory("Fantasy Rampage (needs device work)")
add_subdirectory("Hollow Seeker (crashes)")
add_subdirectory("Jet Pac")
add_subdirectory("Logix")
add_subdirectory("Midnight Wild")
add_subdirectory("Mystic Balloon")
add_subdirectory("Pocket Fighter")
add_subdirectory("Reverse Mermaid Hockey")
add_subdirectory("Shadow Runner")
add_subdirectory("Train Dodge")
add_subdirectory("VIRUS-LQP-79 (broken on device)")