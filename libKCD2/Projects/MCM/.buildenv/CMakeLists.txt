project(MCM LANGUAGES CXX)

find_package(minhook CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)

file(GLOB_RECURSE PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_library(MCM SHARED ${PLUGIN_SOURCES})

target_precompile_headers(MCM PRIVATE "${RE_BUILDENV}/PCH.h")

target_compile_definitions(MCM PRIVATE
    _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)

target_compile_options(MCM PRIVATE /utf-8 /W4 /wd4819 /wd4100)

target_include_directories(MCM PRIVATE
    ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon
    ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_SOURCE_DIR}/../api)

target_link_libraries(MCM PRIVATE kcd_re minhook::minhook pugixml::pugixml)
