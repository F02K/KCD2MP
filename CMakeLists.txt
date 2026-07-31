cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Every DLL loaded into WHGame must use the retail MSVC container/CRT ABI,
# including Debug builds. Debug still retains symbols and disables optimizer
# defaults; it does not switch to the incompatible checked STL.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)

project(KCD2MP VERSION 0.4.0 LANGUAGES CXX C ASM ASM_MASM)

add_compile_definitions(
    "$<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>"
)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(KCD2MP_WHGAME_PATH "" CACHE FILEPATH "Path to WHGame.dll for the offline signature audit")
option(KCD2MP_BUILD_KCSE "Build the bundled libKCD2 KCSE loader and KCD2MP client plugin" ON)

# Fetch modules
message("\nFetching modules")
include(cmake_scripts/git.cmake)
include(cmake_scripts/mimalloc.cmake)
include(cmake_scripts/rom.cmake)
include(cmake_scripts/imguizmo.cmake)
include(cmake_scripts/zip.cmake)

# The bundled pugixml project overrides CMAKE_MSVC_RUNTIME_LIBRARY for its
# Debug target. Put it back on the engine-compatible retail CRT ABI.
if(TARGET pugixml-static)
    set_property(TARGET pugixml-static PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded"
    )
endif()

find_package(Protobuf CONFIG REQUIRED)
find_package(GameNetworkingSockets CONFIG REQUIRED)

if(KCD2MP_BUILD_KCSE)
    find_package(spdlog CONFIG REQUIRED)
    find_package(boost_container CONFIG REQUIRED)

    set(KCD2MP_LIBKCD2_ROOT "${PROJECT_SOURCE_DIR}/libKCD2")
    set(KCD2MP_KCSE_ROOT "${KCD2MP_LIBKCD2_ROOT}/Projects/KCSE")
    if(NOT EXISTS "${KCD2MP_LIBKCD2_ROOT}/.buildenv/PCH.h")
        message(FATAL_ERROR
            "libKCD2 is not initialized. Run "
            "`powershell -ExecutionPolicy Bypass -File tools/init_vendor.ps1` "
            "from the repository root.")
    endif()
    if(NOT EXISTS "${KCD2MP_KCSE_ROOT}/src/dllmain.cpp")
        message(FATAL_ERROR
            "libKCD2's nested KCSE submodule is not initialized. Run "
            "`powershell -ExecutionPolicy Bypass -File tools/init_vendor.ps1` "
            "from the repository root.")
    endif()

    get_target_property(
        KCD2MP_SPDLOG_INCLUDE_DIRECTORIES
        spdlog::spdlog_header_only
        INTERFACE_INCLUDE_DIRECTORIES
    )
    add_library(KCD2MPSpdlogHeaderOnly INTERFACE)
    target_include_directories(KCD2MPSpdlogHeaderOnly INTERFACE
        ${KCD2MP_SPDLOG_INCLUDE_DIRECTORIES}
    )
    target_compile_definitions(KCD2MPSpdlogHeaderOnly INTERFACE
        "FMT_HEADER_ONLY"
        "SPDLOG_FMT_EXTERNAL"
        "SPDLOG_HEADER_ONLY"
    )

    file(GLOB_RECURSE KCD2MP_LIBKCD2_HEADERS CONFIGURE_DEPENDS
        "${KCD2MP_LIBKCD2_ROOT}/include/*.h"
    )
    file(GLOB_RECURSE KCD2MP_LIBKCD2_SOURCES CONFIGURE_DEPENDS
        "${KCD2MP_LIBKCD2_ROOT}/src/*.cpp"
    )
    add_library(kcd_re STATIC
        "${KCD2MP_LIBKCD2_ROOT}/.buildenv/check_sizes.cpp"
        "${KCD2MP_LIBKCD2_ROOT}/.buildenv/CrySDKStubs.cpp"
        ${KCD2MP_LIBKCD2_SOURCES}
        ${KCD2MP_LIBKCD2_HEADERS}
    )
    set_property(TARGET kcd_re PROPERTY CXX_STANDARD 17)
    set_property(TARGET kcd_re PROPERTY CXX_STANDARD_REQUIRED ON)
    target_precompile_headers(kcd_re PRIVATE
        "${KCD2MP_LIBKCD2_ROOT}/.buildenv/PCH.h"
    )
    target_compile_definitions(kcd_re PRIVATE
        "_ITERATOR_DEBUG_LEVEL=0"
        "FORCE_STANDARD_ASSERT"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    target_compile_options(kcd_re PRIVATE /utf-8 /W4 /wd4100 /wd4819)
    target_include_directories(kcd_re PUBLIC
        "${KCD2MP_LIBKCD2_ROOT}/include"
        "${KCD2MP_LIBKCD2_ROOT}/include/CryEngine/CryCommon"
    )
    target_link_libraries(kcd_re PUBLIC Boost::container)

    file(GLOB_RECURSE KCD2MP_KCSE_SOURCES CONFIGURE_DEPENDS
        "${KCD2MP_KCSE_ROOT}/src/*.cpp"
    )
    add_library(KCSE SHARED ${KCD2MP_KCSE_SOURCES})
    set_property(TARGET KCSE PROPERTY CXX_STANDARD 17)
    set_property(TARGET KCSE PROPERTY CXX_STANDARD_REQUIRED ON)
    target_precompile_headers(KCSE PRIVATE
        "${KCD2MP_LIBKCD2_ROOT}/.buildenv/PCH.h"
    )
    target_compile_definitions(KCSE PRIVATE
        "_ITERATOR_DEBUG_LEVEL=0"
        "FORCE_STANDARD_ASSERT"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    target_compile_options(KCSE PRIVATE /utf-8 /W4 /wd4100 /wd4819)
    target_include_directories(KCSE PRIVATE
        "${KCD2MP_LIBKCD2_ROOT}/include"
        "${KCD2MP_LIBKCD2_ROOT}/include/CryEngine/CryCommon"
        "${KCD2MP_KCSE_ROOT}/src"
    )
    target_link_libraries(KCSE PRIVATE
        kcd_re
        KCD2MPSpdlogHeaderOnly
        version.lib
    )
    set_target_properties(KCSE PROPERTIES
        OUTPUT_NAME "dinput8"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
    )
endif()

if (TARGET GameNetworkingSockets::GameNetworkingSockets)
    set(KCD2MP_GNS_TARGET GameNetworkingSockets::GameNetworkingSockets)
elseif (TARGET GameNetworkingSockets::GameNetworkingSockets_s)
    set(KCD2MP_GNS_TARGET GameNetworkingSockets::GameNetworkingSockets_s)
elseif (TARGET GameNetworkingSockets)
    set(KCD2MP_GNS_TARGET GameNetworkingSockets)
else()
    message(FATAL_ERROR "The GameNetworkingSockets package did not export a supported CMake target.")
endif()

message(STATUS "KCD2MP")
file(GLOB_RECURSE SRC_MAIN CONFIGURE_DEPENDS
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
    "${SRC_DIR}/**.def"
    "${SRC_DIR}/**.rc"
)
# Ignore files generated by older checkouts, which wrote version.cpp into src/.
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]version\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]signatures[/\\\\]signature_core\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]multiplayer[/\\\\]protocol\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]multiplayer[/\\\\]networking\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]server[/\\\\].*")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]npc[/\\\\]catalog\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]npc[/\\\\]equipment_catalog\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]kcse[/\\\\]plugin\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]kcse[/\\\\]native_.*\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]multiplayer[/\\\\]client\\.cpp$")
list(FILTER SRC_MAIN EXCLUDE REGEX "[/\\\\]multiplayer[/\\\\]identity_store\\.cpp$")

if (NOT TARGET Zydis)
    message(FATAL_ERROR
        "KCD2MP requires the Zydis target supplied by the pinned "
        "ReturnOfModdingBase/PolyHook dependency.")
endif()

add_library(KCD2MPSignatureCore STATIC
    "${SRC_DIR}/signatures/signature_core.cpp"
    "${SRC_DIR}/signatures/signature_core.hpp"
)
set_property(TARGET KCD2MPSignatureCore PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPSignatureCore PUBLIC "${SRC_DIR}")
target_compile_definitions(KCD2MPSignatureCore PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
target_link_libraries(KCD2MPSignatureCore PUBLIC Zydis)
set_property(TARGET KCD2MPSignatureCore PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_options("$<$<COMPILE_LANGUAGE:C,CXX>:/bigobj>")
add_compile_options("$<$<COMPILE_LANG_AND_ID:C,MSVC>:/utf-8>")
add_compile_options("$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/utf-8>")

if (FINAL)
    add_compile_definitions(FINAL)
endif()

add_library(KCD2MPNpcCatalog STATIC
    "${SRC_DIR}/npc/catalog.cpp"
    "${SRC_DIR}/npc/catalog.hpp"
    "${SRC_DIR}/npc/equipment_catalog.cpp"
    "${SRC_DIR}/npc/equipment_catalog.hpp"
)
set_property(TARGET KCD2MPNpcCatalog PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPNpcCatalog PUBLIC
    "${SRC_DIR}"
    "${zip_SOURCE_DIR}"
)
target_link_libraries(KCD2MPNpcCatalog PUBLIC
    pugixml::pugixml
    nlohmann_json::nlohmann_json
    zip
)
target_compile_definitions(KCD2MPNpcCatalog PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
set_property(TARGET KCD2MPNpcCatalog PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_library(KCD2MPProtocol STATIC
    "${SRC_DIR}/multiplayer/protocol.cpp"
    "${SRC_DIR}/multiplayer/protocol.hpp"
    "${PROJECT_SOURCE_DIR}/protocol/kcd2mp.proto"
)
set_property(TARGET KCD2MPProtocol PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPProtocol PUBLIC
    "${SRC_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(KCD2MPProtocol PUBLIC protobuf::libprotobuf)
target_compile_definitions(KCD2MPProtocol PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
set_property(TARGET KCD2MPProtocol PROPERTY COMPILE_WARNING_AS_ERROR ON)
protobuf_generate(
    TARGET KCD2MPProtocol
    LANGUAGE cpp
    PROTOS "${PROJECT_SOURCE_DIR}/protocol/kcd2mp.proto"
    IMPORT_DIRS "${PROJECT_SOURCE_DIR}/protocol"
)

add_library(KCD2MPNetworking STATIC
    "${SRC_DIR}/multiplayer/networking.cpp"
    "${SRC_DIR}/multiplayer/networking.hpp"
)
set_property(TARGET KCD2MPNetworking PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPNetworking PUBLIC "${SRC_DIR}")
target_link_libraries(KCD2MPNetworking PUBLIC KCD2MPProtocol ${KCD2MP_GNS_TARGET})
target_compile_definitions(KCD2MPNetworking PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
set_property(TARGET KCD2MPNetworking PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_library(KCD2MPServerCore STATIC
    "${SRC_DIR}/server/server_config.cpp"
    "${SRC_DIR}/server/server_config.hpp"
    "${SRC_DIR}/server/server_core.cpp"
    "${SRC_DIR}/server/server_core.hpp"
    "${SRC_DIR}/server/starter_profile.cpp"
    "${SRC_DIR}/server/starter_profile.hpp"
    "${SRC_DIR}/server/world_store.cpp"
    "${SRC_DIR}/server/world_store.hpp"
)
set_property(TARGET KCD2MPServerCore PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPServerCore PUBLIC "${SRC_DIR}")
target_link_libraries(KCD2MPServerCore PUBLIC
    KCD2MPProtocol
    tomlplusplus::tomlplusplus
    bcrypt
)
target_compile_definitions(KCD2MPServerCore PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
set_property(TARGET KCD2MPServerCore PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_executable(KCD2MPServer "${SRC_DIR}/server/server_main.cpp")
set_property(TARGET KCD2MPServer PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPServer PRIVATE "${SRC_DIR}")
target_link_libraries(KCD2MPServer PRIVATE
    KCD2MPServerCore
    KCD2MPNetworking
)
target_compile_definitions(KCD2MPServer PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
set_property(TARGET KCD2MPServer PROPERTY COMPILE_WARNING_AS_ERROR ON)
add_custom_command(TARGET KCD2MPServer POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/server.toml.example"
        "$<TARGET_FILE_DIR:KCD2MPServer>/server.toml.example"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/starter_profile.toml"
        "$<TARGET_FILE_DIR:KCD2MPServer>/starter_profile.toml"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/data/npc_archetypes.json"
        "$<TARGET_FILE_DIR:KCD2MPServer>/npc_archetypes.json"
)

add_library(KCD2MP MODULE "${SRC_MAIN}" "${GENERATED_VERSION_SOURCE}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET KCD2MP PROPERTY CXX_STANDARD 23)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})
source_group("generated" FILES "${GENERATED_VERSION_SOURCE}")

target_include_directories(KCD2MP PRIVATE
    "${SRC_DIR}"
    "${imguizmo_SOURCE_DIR}"
    "${zip_SOURCE_DIR}"
)

target_precompile_headers(KCD2MP PRIVATE "${SRC_DIR}/common.hpp")

target_link_libraries(KCD2MP PRIVATE
    ReturnOfModdingBase
    KCD2MPProtocol
    KCD2MPSignatureCore
    KCD2MPNpcCatalog
    Zydis
    zip
    imguizmo
    mimalloc-static
)

if(KCD2MP_BUILD_KCSE)
    add_library(KCD2MPKCSENativeRuntime OBJECT
        "${SRC_DIR}/kcse/native_entity_backend.cpp"
        "${SRC_DIR}/kcse/native_entity_backend.hpp"
        "${SRC_DIR}/kcse/native_profile_backend.cpp"
        "${SRC_DIR}/kcse/native_profile_backend.hpp"
        "${SRC_DIR}/kcse/native_remote_avatar_backend.cpp"
        "${SRC_DIR}/kcse/native_remote_avatar_backend.hpp"
        "${SRC_DIR}/kcse/native_runtime.cpp"
        "${SRC_DIR}/kcse/native_runtime.hpp"
    )
    set_property(TARGET KCD2MPKCSENativeRuntime PROPERTY CXX_STANDARD 23)
    set_property(TARGET KCD2MPKCSENativeRuntime PROPERTY CXX_STANDARD_REQUIRED ON)
    target_precompile_headers(KCD2MPKCSENativeRuntime PRIVATE
        "${PROJECT_SOURCE_DIR}/libKCD2/.buildenv/PCH.h"
    )
    target_include_directories(KCD2MPKCSENativeRuntime PRIVATE
        "${SRC_DIR}"
        "${PROJECT_SOURCE_DIR}/libKCD2/include"
        "${PROJECT_SOURCE_DIR}/libKCD2/include/CryEngine/CryCommon"
    )
    target_compile_definitions(KCD2MPKCSENativeRuntime PRIVATE
        "_ITERATOR_DEBUG_LEVEL=0"
        "FORCE_STANDARD_ASSERT"
        "_CRT_SECURE_NO_WARNINGS"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    # CryEngine's legacy SDK headers still rely on MSVC's delayed template
    # lookup. Keep the runtime on C++23 while retaining that source-compatible
    # lookup mode; this does not alter generated ABI.
    target_compile_options(KCD2MPKCSENativeRuntime PRIVATE
        /utf-8 /W4 /wd4100 /wd4819 /permissive
    )
    target_link_libraries(KCD2MPKCSENativeRuntime PRIVATE
        kcd_re
        KCD2MPNpcCatalog
        KCD2MPProtocol
    )
    set_property(TARGET KCD2MPKCSENativeRuntime PROPERTY COMPILE_WARNING_AS_ERROR ON)

    add_library(KCD2MPKCSEClient SHARED
        "${SRC_DIR}/kcse/client_api.hpp"
        "${SRC_DIR}/kcse/join_trace.cpp"
        "${SRC_DIR}/kcse/join_trace.hpp"
        "${SRC_DIR}/kcse/plugin.cpp"
        "${SRC_DIR}/kcse/native_runtime.hpp"
        "${SRC_DIR}/multiplayer/client.cpp"
        "${SRC_DIR}/multiplayer/client.hpp"
        "${SRC_DIR}/multiplayer/identity_store.cpp"
        "${SRC_DIR}/multiplayer/identity_store.hpp"
    )
    set_property(TARGET KCD2MPKCSEClient PROPERTY CXX_STANDARD 23)
    set_property(TARGET KCD2MPKCSEClient PROPERTY CXX_STANDARD_REQUIRED ON)
    target_include_directories(KCD2MPKCSEClient PRIVATE
        "${SRC_DIR}"
        "${PROJECT_SOURCE_DIR}/libKCD2/include"
        "${PROJECT_SOURCE_DIR}/libKCD2/include/CryEngine/CryCommon"
    )
    target_compile_definitions(KCD2MPKCSEClient PRIVATE
        "_ITERATOR_DEBUG_LEVEL=0"
        "FORCE_STANDARD_ASSERT"
        "_CRT_SECURE_NO_WARNINGS"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    target_compile_options(KCD2MPKCSEClient PRIVATE /utf-8 /W4 /wd4100 /wd4819)
    target_link_libraries(KCD2MPKCSEClient PRIVATE
        KCD2MPKCSENativeRuntime
        kcd_re
        KCD2MPProtocol
        KCD2MPNetworking
        KCD2MPNpcCatalog
        crypt32
    )
    set_property(TARGET KCD2MPKCSEClient PROPERTY COMPILE_WARNING_AS_ERROR ON)

    add_custom_target(KCD2MPRuntime
        DEPENDS KCD2MP KCSE KCD2MPKCSEClient
    )
else()
    add_custom_target(KCD2MPRuntime DEPENDS KCD2MP)
endif()

# Warnings as errors
set_property(TARGET KCD2MP PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_compile_definitions(KCD2MP PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

if (MSVC)
    if (FINAL)
        # Aggresive inlining (Release)
        string(REPLACE "Ob2" "Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
        # Aggresive inlining (RelWithDebInfo)
        string(REPLACE "Ob1" "Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
        # Enable intrinsics
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi")
        # Favor fast code
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ot")
        # -ffast-math (fused multiply-add if the arch allows)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
        # Whole program optimization
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        # Function-level linking
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy")
        # Disable Buffer Security Check
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS-")
        # Package global data in COMDAT sections for optimization.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gw")

        # For all objects:
        # - strip unused functions
        # - fold identical functions
        # - link-time code generation
        string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
        string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")
        string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO}")
        set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /OPT:ICF,REF /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /OPT:ICF,REF /LTCG")
        set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO} /OPT:ICF,REF /LTCG")

        set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /PDBALTPATH:d3d12.pdb")
        set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /PDBALTPATH:d3d12.pdb")
        set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO} /PDBALTPATH:d3d12.pdb")
    endif()
endif()

set_target_properties(KCD2MP PROPERTIES OUTPUT_NAME "d3d12_")

add_executable(KCD2MPSignatureAudit "${PROJECT_SOURCE_DIR}/tools/signature_audit_main.cpp")
set_property(TARGET KCD2MPSignatureAudit PROPERTY CXX_STANDARD 23)
target_include_directories(KCD2MPSignatureAudit PRIVATE "${SRC_DIR}")
target_compile_definitions(KCD2MPSignatureAudit PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)
target_link_libraries(KCD2MPSignatureAudit PRIVATE KCD2MPSignatureCore bcrypt)
set_property(TARGET KCD2MPSignatureAudit PROPERTY COMPILE_WARNING_AS_ERROR ON)

include(CTest)
if (BUILD_TESTING)
    add_executable(KCD2MPSignatureCoreTests "${PROJECT_SOURCE_DIR}/tests/signature_core_tests.cpp")
    set_property(TARGET KCD2MPSignatureCoreTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPSignatureCoreTests PRIVATE "${SRC_DIR}")
    target_compile_definitions(KCD2MPSignatureCoreTests PRIVATE
        "_CRT_SECURE_NO_WARNINGS"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    target_link_libraries(KCD2MPSignatureCoreTests PRIVATE KCD2MPSignatureCore)
    set_property(TARGET KCD2MPSignatureCoreTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPSignatureCoreTests COMMAND KCD2MPSignatureCoreTests)

    add_executable(KCD2MPEnginePathTests "${PROJECT_SOURCE_DIR}/tests/engine_path_tests.cpp")
    set_property(TARGET KCD2MPEnginePathTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPEnginePathTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPEnginePathTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPEnginePathTests COMMAND KCD2MPEnginePathTests)

    add_executable(KCD2MPProtocolTests "${PROJECT_SOURCE_DIR}/tests/protocol_tests.cpp")
    set_property(TARGET KCD2MPProtocolTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPProtocolTests PRIVATE KCD2MPProtocol)
    set_property(TARGET KCD2MPProtocolTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPProtocolTests COMMAND KCD2MPProtocolTests)

    add_executable(KCD2MPServerCoreTests "${PROJECT_SOURCE_DIR}/tests/server_core_tests.cpp")
    set_property(TARGET KCD2MPServerCoreTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPServerCoreTests PRIVATE KCD2MPServerCore)
    target_compile_definitions(KCD2MPServerCoreTests PRIVATE
        "KCD2MP_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
    )
    set_property(TARGET KCD2MPServerCoreTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPServerCoreTests COMMAND KCD2MPServerCoreTests)

    add_executable(KCD2MPGameCommandQueueTests
        "${PROJECT_SOURCE_DIR}/tests/game_command_queue_tests.cpp"
    )
    set_property(TARGET KCD2MPGameCommandQueueTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPGameCommandQueueTests PRIVATE KCD2MPProtocol)
    target_include_directories(KCD2MPGameCommandQueueTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPGameCommandQueueTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPGameCommandQueueTests COMMAND KCD2MPGameCommandQueueTests)

    add_executable(KCD2MPEntityControlTests
        "${PROJECT_SOURCE_DIR}/tests/entity_control_tests.cpp"
    )
    set_property(TARGET KCD2MPEntityControlTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPEntityControlTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPEntityControlTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPEntityControlTests COMMAND KCD2MPEntityControlTests)

    add_executable(KCD2MPRuntimeHandleTests
        "${PROJECT_SOURCE_DIR}/tests/runtime_handle_tests.cpp"
    )
    set_property(TARGET KCD2MPRuntimeHandleTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPRuntimeHandleTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPRuntimeHandleTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPRuntimeHandleTests COMMAND KCD2MPRuntimeHandleTests)

    add_executable(KCD2MPRemoteAvatarTests
        "${PROJECT_SOURCE_DIR}/tests/remote_avatar_tests.cpp"
    )
    set_property(TARGET KCD2MPRemoteAvatarTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPRemoteAvatarTests PRIVATE KCD2MPProtocol)
    target_include_directories(KCD2MPRemoteAvatarTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPRemoteAvatarTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPRemoteAvatarTests COMMAND KCD2MPRemoteAvatarTests)

    add_executable(KCD2MPNpcTests
        "${PROJECT_SOURCE_DIR}/tests/npc_tests.cpp"
    )
    set_property(TARGET KCD2MPNpcTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPNpcTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPNpcTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPNpcTests COMMAND KCD2MPNpcTests)

    add_executable(KCD2MPKCSEClientApiTests
        "${PROJECT_SOURCE_DIR}/tests/client_api_tests.cpp"
    )
    set_property(TARGET KCD2MPKCSEClientApiTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPKCSEClientApiTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPKCSEClientApiTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPKCSEClientApiTests COMMAND KCD2MPKCSEClientApiTests)

    add_executable(KCD2MPNpcCatalogTests
        "${PROJECT_SOURCE_DIR}/tests/npc_catalog_tests.cpp"
    )
    set_property(TARGET KCD2MPNpcCatalogTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPNpcCatalogTests PRIVATE KCD2MPNpcCatalog)
    target_compile_definitions(KCD2MPNpcCatalogTests PRIVATE
        "KCD2MP_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
    )
    set_property(TARGET KCD2MPNpcCatalogTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPNpcCatalogTests COMMAND KCD2MPNpcCatalogTests)

    add_executable(KCD2MPEquipmentCatalogTests
        "${PROJECT_SOURCE_DIR}/tests/equipment_catalog_tests.cpp"
    )
    set_property(TARGET KCD2MPEquipmentCatalogTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPEquipmentCatalogTests PRIVATE KCD2MPNpcCatalog)
    set_property(TARGET KCD2MPEquipmentCatalogTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPEquipmentCatalogTests COMMAND KCD2MPEquipmentCatalogTests)

    add_executable(KCD2MPAvatarVisualTests
        "${PROJECT_SOURCE_DIR}/tests/avatar_visual_tests.cpp"
    )
    set_property(TARGET KCD2MPAvatarVisualTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPAvatarVisualTests PRIVATE KCD2MPProtocol)
    target_include_directories(KCD2MPAvatarVisualTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPAvatarVisualTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPAvatarVisualTests COMMAND KCD2MPAvatarVisualTests)

    add_executable(KCD2MPProfileReconcilerTests
        "${PROJECT_SOURCE_DIR}/tests/profile_reconciler_tests.cpp"
    )
    set_property(TARGET KCD2MPProfileReconcilerTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPProfileReconcilerTests PRIVATE
        KCD2MPServerCore
    )
    target_include_directories(KCD2MPProfileReconcilerTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPProfileReconcilerTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPProfileReconcilerTests COMMAND KCD2MPProfileReconcilerTests)

    add_executable(KCD2MPEngineConsoleQueueTests
        "${PROJECT_SOURCE_DIR}/tests/engine_console_queue_tests.cpp"
    )
    set_property(TARGET KCD2MPEngineConsoleQueueTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPEngineConsoleQueueTests PRIVATE "${SRC_DIR}")
    set_property(TARGET KCD2MPEngineConsoleQueueTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPEngineConsoleQueueTests COMMAND KCD2MPEngineConsoleQueueTests)

    add_executable(KCD2MPNetworkingTests
        "${PROJECT_SOURCE_DIR}/tests/networking_tests.cpp"
    )
    set_property(TARGET KCD2MPNetworkingTests PROPERTY CXX_STANDARD 23)
    target_link_libraries(KCD2MPNetworkingTests PRIVATE KCD2MPNetworking)
    set_property(TARGET KCD2MPNetworkingTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPNetworkingTests COMMAND KCD2MPNetworkingTests)

    add_executable(KCD2MPIdentityStoreTests
        "${PROJECT_SOURCE_DIR}/tests/identity_store_tests.cpp"
        "${SRC_DIR}/multiplayer/identity_store.cpp"
        "${SRC_DIR}/multiplayer/identity_store.hpp"
    )
    set_property(TARGET KCD2MPIdentityStoreTests PROPERTY CXX_STANDARD 23)
    target_include_directories(KCD2MPIdentityStoreTests PRIVATE "${SRC_DIR}")
    target_compile_definitions(KCD2MPIdentityStoreTests PRIVATE
        "_CRT_SECURE_NO_WARNINGS"
        "NOMINMAX"
        "WIN32_LEAN_AND_MEAN"
    )
    target_link_libraries(KCD2MPIdentityStoreTests PRIVATE crypt32)
    set_property(TARGET KCD2MPIdentityStoreTests PROPERTY COMPILE_WARNING_AS_ERROR ON)
    add_test(NAME KCD2MPIdentityStoreTests COMMAND KCD2MPIdentityStoreTests)

    add_custom_target(KCD2MPTests
        DEPENDS
            KCD2MPSignatureCoreTests
            KCD2MPEnginePathTests
            KCD2MPProtocolTests
            KCD2MPServerCoreTests
            KCD2MPGameCommandQueueTests
            KCD2MPEntityControlTests
            KCD2MPRuntimeHandleTests
            KCD2MPRemoteAvatarTests
            KCD2MPNpcTests
            KCD2MPKCSEClientApiTests
            KCD2MPNpcCatalogTests
            KCD2MPEquipmentCatalogTests
            KCD2MPAvatarVisualTests
            KCD2MPProfileReconcilerTests
            KCD2MPEngineConsoleQueueTests
            KCD2MPNetworkingTests
            KCD2MPIdentityStoreTests
    )
endif()

if (KCD2MP_WHGAME_PATH)
    add_custom_target(KCD2MPAuditInstalledGame
        COMMAND "$<TARGET_FILE:KCD2MPSignatureAudit>" "${KCD2MP_WHGAME_PATH}"
        DEPENDS KCD2MPSignatureAudit
        COMMENT "Auditing KCD2MP signatures against WHGame.dll"
        VERBATIM
    )
endif()
