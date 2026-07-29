project(KCSE LANGUAGES CXX)

if(NOT DEFINED RE_ROOT)
    get_filename_component(RE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
endif()
if(NOT DEFINED RE_BUILDENV)
    set(RE_BUILDENV "${RE_ROOT}/.buildenv")
endif()

file(GLOB_RECURSE KCSE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_library(KCSE SHARED ${KCSE_SOURCES})

set_property(TARGET KCSE PROPERTY CXX_STANDARD 17)
set_property(TARGET KCSE PROPERTY CXX_STANDARD_REQUIRED ON)
target_precompile_headers(KCSE PRIVATE "${RE_BUILDENV}/PCH.h")

target_compile_definitions(KCSE PRIVATE
    _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)

target_compile_options(KCSE PRIVATE /utf-8 /W4 /wd4819 /wd4100)

target_include_directories(KCSE PRIVATE
    ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon ${CMAKE_CURRENT_SOURCE_DIR}/../src)

if(NOT DEFINED LIBKCD2_SPDLOG_TARGET)
    set(LIBKCD2_SPDLOG_TARGET spdlog::spdlog)
endif()
target_link_libraries(KCSE PRIVATE kcd_re ${LIBKCD2_SPDLOG_TARGET} version.lib)

# Loaded as a dinput8.dll proxy from the game's Bin/Win64.
set_target_properties(KCSE PROPERTIES OUTPUT_NAME "dinput8")
