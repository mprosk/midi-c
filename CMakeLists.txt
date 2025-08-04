# ============================================================================
# PROJECT SETUP
# ============================================================================

cmake_minimum_required(VERSION 3.10)

project(midi-c C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g \
    -Wall \
    -Wextra \
    -pedantic \
    -Werror \
    -Winit-self \
    -Wswitch-default")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# ============================================================================
# INCLUDE DIRECTORIES
# ============================================================================

# Include directories
include_directories(midi)

# ============================================================================
# MIDI Static Library
# ============================================================================

# MIDI Library
add_library(midi_lib STATIC
    midi/midi.c
)

# Set library properties
set_target_properties(midi_lib PROPERTIES
    OUTPUT_NAME midi
    PUBLIC_HEADER midi.h
)

# Include directories for this library
target_include_directories(midi_lib PUBLIC 
    midi
)

# ============================================================================
# Unity Static Library
# ============================================================================

# Create Unity library
add_library(unity_lib STATIC
    test/unity/unity.c
)

# Include test directories for Unity library headers
target_include_directories(unity_lib PUBLIC 
    test/unity
)

# ============================================================================
# MIDI Test Executable
# ============================================================================

# Test executable for MIDI
add_executable(test_midi
    test/test_midi.c
)

# Link the MIDI library and Unity library to the test executable
target_link_libraries(test_midi
    midi_lib
    unity_lib
)

# Include test directories for headers
target_include_directories(test_midi PRIVATE 
    test
    midi
)

# ============================================================================
# TESTING CONFIGURATION
# ============================================================================

# Enable testing
enable_testing()
add_test(NAME midi_tests COMMAND test_midi)
