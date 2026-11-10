cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Synchronize mod version from git tag
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(GIT_TAG_VERSION)
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mod.json" CURRENT_MOD_JSON)
        string(JSON CURRENT_VER GET "${CURRENT_MOD_JSON}" "version")
        if(NOT "${CURRENT_VER}" STREQUAL "${GIT_TAG_VERSION}")
            string(JSON NEW_MOD_JSON SET "${CURRENT_MOD_JSON}" "version" "\"${GIT_TAG_VERSION}\"")
            file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/mod.json" "${NEW_MOD_JSON}")
            message(STATUS "Synchronized mod.json version to git tag: ${GIT_TAG_VERSION}")
        endif()
    endif()
endif()

project(MusicCleaner)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME}_PACKAGE POST_BUILD
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/bundle_assets.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Adding screenshot assets to .geode package"
)
