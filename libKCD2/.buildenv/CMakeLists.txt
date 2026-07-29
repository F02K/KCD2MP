cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "")

project(KCD2_RE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths shared with subprojects
set(RE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(RE_BUILDENV "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(spdlog CONFIG REQUIRED)
find_package(boost_container CONFIG REQUIRED)   # boost::container::vector / flat_map (the game's sorted containers)

# ---- kcd_re static library (libKCD2) ----
# Same target name as libKCD1 ("kcd_re") so plugin CMakeLists port unchanged;
# the two libraries live in separate trees (KCD1/RE vs KCD2/RE) and build apart.
file(GLOB_RECURSE RE_HEADERS "${RE_ROOT}/include/*.h")
file(GLOB_RECURSE RE_SOURCES "${RE_ROOT}/src/*.cpp")

# CrySDKStubs.cpp provides the CryEngine SDK glue (allocators, critical sections)
# the vendored CryCommon headers reference; CRT/Win32-backed (no address library).
add_library(kcd_re STATIC check_sizes.cpp CrySDKStubs.cpp ${RE_SOURCES} ${RE_HEADERS})

target_precompile_headers(kcd_re PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/PCH.h")

target_compile_definitions(kcd_re PRIVATE
    _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)

target_compile_options(kcd_re PRIVATE /utf-8 /W4 /wd4819 /wd4100)

target_include_directories(kcd_re PRIVATE
    ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon)

target_link_libraries(kcd_re PUBLIC Boost::container)

# ---- Auto-discover subprojects under Projects/*/.buildenv ----
file(GLOB PLUGIN_DIRS "${RE_ROOT}/Projects/*")
foreach(PLUGIN_DIR ${PLUGIN_DIRS})
    if(EXISTS "${PLUGIN_DIR}/.buildenv/CMakeLists.txt")
        get_filename_component(PLUGIN_NAME ${PLUGIN_DIR} NAME)
        message(STATUS "Subproject: ${PLUGIN_NAME}")
        add_subdirectory("${PLUGIN_DIR}/.buildenv" "${CMAKE_BINARY_DIR}/${PLUGIN_NAME}")
    endif()
endforeach()

# ---- Class layout dump ----
if(DEFINED CLASS_LAYOUT)
    if(CLASS_LAYOUT STREQUAL "ALL")
        target_compile_options(kcd_re PRIVATE /d1reportAllClassLayout)
    else()
        target_compile_options(kcd_re PRIVATE /d1reportSingleClassLayout${CLASS_LAYOUT})
    endif()
endif()
