project(KCSE LANGUAGES CXX)

file(GLOB_RECURSE KCSE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_library(KCSE SHARED ${KCSE_SOURCES})

target_precompile_headers(KCSE PRIVATE "${RE_BUILDENV}/PCH.h")

target_compile_definitions(KCSE PRIVATE
    _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)

target_compile_options(KCSE PRIVATE /utf-8 /W4 /wd4819 /wd4100)

target_include_directories(KCSE PRIVATE
    ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(KCSE PRIVATE kcd_re spdlog::spdlog version.lib)

# Loaded as a dinput8.dll proxy from the game's Bin/Win64.
set_target_properties(KCSE PROPERTIES OUTPUT_NAME "dinput8")
