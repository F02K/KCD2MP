project(CompassOverhaul LANGUAGES CXX)

find_package(minhook CONFIG REQUIRED)

file(GLOB_RECURSE PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${RE_BUILDENV}/PCH.h")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)

target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /W4 /wd4819 /wd4100)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(${PROJECT_NAME} PRIVATE kcd_re minhook::minhook)

# ---- Auto-deploy (uncomment + adjust path to copy into the game's mod folder) ----
# set(PLUGIN_DEPLOY "C:/Program Files (x86)/Steam/steamapps/common/KingdomComeDeliverance2/mods/CompassOverhaul/KCSE/Plugins")
# add_custom_command(TARGET CompassOverhaul POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_DEPLOY}"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:CompassOverhaul>" "${PLUGIN_DEPLOY}/"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:CompassOverhaul>" "${PLUGIN_DEPLOY}/"
#     COMMENT "Deploying CompassOverhaul.dll + .pdb to ${PLUGIN_DEPLOY}"
# )
