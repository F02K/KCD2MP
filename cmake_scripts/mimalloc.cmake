include(FetchContent)

set(MI_OVERRIDE OFF)
set(MI_BUILD_TESTS OFF CACHE BOOL "Do not build mimalloc's test executables" FORCE)

FetchContent_Declare(
	mimalloc
	GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG        v2.2.3
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(mimalloc)
